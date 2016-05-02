#include "bodyParts.hpp"

#include <set>

#include "creature.hpp"
#include "world.hpp"
#include "pheromoneMap.hpp"
#include "obstacle.hpp"
#include "point.hpp"


AntLegs::AntLegs(World* w, Creature* owner) : BodyPart(w,owner)
{
    targetPos_=owner->getPos();
    timeNotMoving_=0;
}

void AntLegs::goToPos(const Point& p){
    targetPos_=p;
}

void AntLegs::goRandom(){
    targetPos_=Point(rand()-RAND_MAX/2, rand()-RAND_MAX/2);
}

void AntLegs::step(int deltatime){
    while((deltatime--)>0){
        Point curPos=owner_->getPos();

        // check if this position is free (there is no other creature)
        // (detect collision)

        for(int i=0;i<=2;++i){
            // try to move by both axes, then single axes if not possible
            bool dx,dy;
            if(i==0){
                dx=1;
                dy=1;
            }else if(i==1){
                dx=1;
                dy=0;
            }else if(i==2){
                dx=0;
                dy=1;
            }

            int x=curPos.posX();
            int y=curPos.posY();

            if(dx && curPos.posX() != targetPos_.posX())
                x+= (curPos.posX() < targetPos_.posX()) ? 1 : -1;
            if(dy && curPos.posY() != targetPos_.posY())
                y+= (curPos.posY() < targetPos_.posY()) ? 1 : -1;

            if(!Point(x,y).isInBounds(world_->getDimensions()))
                continue;

            bool collision_detected = false;
            for(const auto& creature : world_->getSimulationObjects<Creature>())
            {
                if(creature->getPos() == Point(x,y))
                {
                    collision_detected = true;
                    break;
                }
            }
            for(const auto& obstacle : world_->getSimulationObjects<Obstacle>())
            {
                if(obstacle->getPos() == Point(x,y))
                {
                    collision_detected = true;
                    break;
                }
            }

            if(!collision_detected)
            {
                owner_->setPos(Point(x,y));
                timeNotMoving_=0;
                return;
            }
        }
        ++timeNotMoving_;
    }
}

// AntSensor

const float AntSensor::pheromoneRange=5.5;

Point AntSensor::Observation::getPos()const{ 
    return ent_.lock()->getPos(); 
}

int AntSensor::Observation::getSmell()const{
    return ent_.lock()->getSmell(); 
}

std::vector<AntSensor::Observation> AntSensor::getEntities(){
    std::vector<Observation> ret;
    
    for(const auto& a : world_->getEntityPtrs()){
        if(a.lock()->getPos().getDistance(owner_->getPos()) <= 4)
            ret.push_back(Observation(a));
    }
    
    Point ownerPos=owner_->getPos();
    std::sort(ret.begin(),ret.end(),
        [ownerPos] (const Observation& a,const Observation& b) -> bool 
        { return ownerPos.getDistance(a.getPos()) 
            < ownerPos.getDistance(b.getPos()); });
    
    return ret;
}

Point AntSensor::getClosestAnthillPheromone(float distance){
    float range=pheromoneRange;
    int r=int(range+1);
    Point ownPos=owner_->getPos();

    Point bestFit=Point(INT_MAX,INT_MAX);

    for(const auto pm : world_->getSimulationObjects<PheromoneMap>()){
        if(pm->getType() != PheromoneMap::Type::Anthill)
            continue;

        for(int dx=-r; dx<=r; ++dx){
            for(int dy=-r; dy<=r; ++dy){

                int x=ownPos.posX()+dx;
                int y=ownPos.posY()+dy;
                Point pos=Point(x,y);

                if(!pos.isInBounds(world_->getDimensions())) 
                    continue;

                if(pos.getDistance(ownPos)<distance)
                    continue;

                if(pm->getStrengthAtPosition(pos)<0.1)
                    continue;

                if(pos.getDistance(ownPos) < bestFit.getDistance(ownPos)){
                    bestFit=pos;
                }
            }
        }
    }

    return bestFit;
}

Point AntSensor::getFarthestAnthillPheromone(float distance){
    //TODO: implement
    assert(0);
    return Point(0,0);
}

float AntSensor::getAnthillPheromoneStrength(Point pos){
    for(const auto pm : world_->getSimulationObjects<PheromoneMap>()){
        if(pm->getType() != PheromoneMap::Type::Anthill)
            continue;
        if(owner_->getPos().getDistance(pos)<=pheromoneRange)
            return pm->getStrengthAtPosition(pos);
        else 
            return 0;
    }
    throw std::runtime_error("AntSensor::getAntillPheromoneStrength: no AnthillPheromone map");
    return 0;
}

// AntMandibles
boost::weak_ptr<Entity> AntMandibles::getHoldingObject() const
{
	return holdingObject_;
}

bool AntMandibles::grab(boost::weak_ptr<Entity> e){
    
    if(isHolding())
        return false;
    
    // TODO:
    // can grab from adjecent positions
    std::set<Point> grab_from;
    const auto& my_pos = owner_->getPos();
    grab_from.insert(my_pos + Point(1, 0));
    grab_from.insert(my_pos + Point(-1, 0));
    grab_from.insert(my_pos + Point(0, 1));
    grab_from.insert(my_pos + Point(0, -1));
    grab_from.insert(my_pos + Point(0, 0));
    
    for (const auto& pos : grab_from)
    {
        if (pos == e.lock()->getPos())
        {
            holdingObject_ = e;
            break;
        }
    }
    return true;
}

bool AntMandibles::grab(AntSensor::Observation o){
    grab(o.ent_);
    return 1;
}

bool AntMandibles::drop(){
    if(holdingObject_.expired())
        return false;
    else{
        holdingObject_=boost::weak_ptr<Entity>();
        return true;
    }
}

void AntMandibles::step(int deltaTime){
    if(isHolding()){
        holdingObject_.lock()->setPos(owner_->getPos());
	}
}

void AntMandibles::accept(Visitor &v) const
{
	v.visit(*this);
}

// AntWorkerAbdomen
void AntWorkerAbdomen::dropToFoodPheromones(){
    dropType=PheromoneMap::Type::ToFood;
}
void AntWorkerAbdomen::dropFromFoodPheromones(){
    dropType=PheromoneMap::Type::FromFood;
}

void AntWorkerAbdomen::step(int deltaTime){
    if(deltaTime<=0)
        return;
    if(dropType == PheromoneMap::Type::None)
        return;
    for(const auto& pm : world_->getSimulationObjects<PheromoneMap>()){
        if(pm->getType()==dropType){
            pm->createBlob(owner_->getPos(), 2, 100);
            return;
        }
    }

    dropType = PheromoneMap::Type::None;
}


// AntQueenAbdomen
void AntQueenAbdomen::dropAnthillPheromones(){
    dropType=PheromoneMap::Type::Anthill;
}

void AntQueenAbdomen::step(int deltaTime){
    if(deltaTime<=0)
        return;
    if(dropType == PheromoneMap::Type::None)
        return;
    for(auto pm : world_->getSimulationObjects<PheromoneMap>()){
        if(pm->getType()==dropType){
            pm->createBlob(owner_->getPos(), 6, 200);
            return;
        }
    }

    dropType = PheromoneMap::Type::None;
}
