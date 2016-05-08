#include "bodyParts.hpp"

#include <set>

#include "creature.hpp"
#include "world.hpp"
#include "pheromoneMap.hpp"
#include "obstacle.hpp"
#include "point.hpp"

// BodyPart
bool BodyPart::isAccessible(Point p){
    if(!p.isInBounds(world_->getDimensions()))
        return false;

    bool collision_detected = false;
    for(const auto& creature : world_->getSimulationObjects<Creature>()){
        if(creature->getPos() == p && creature->hasCollision()){
            collision_detected = true;
            break;
        }
    }
    if(collision_detected)
        return false;
    for(const auto& obstacle : world_->getSimulationObjects<Obstacle>()){
        if(obstacle->getPos() == p){
            collision_detected = true;
            break;
        }
    }
    return !collision_detected;
}


// AntLegs
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

            if(BodyPart::isAccessible(Point(x,y))){
                owner_->setPos(Point(x,y));
                timeNotMoving_=0;
                return;
            }
        }
        ++timeNotMoving_;
    }
}

// AntSensor

const float AntSensor::pheromoneRange=7.5;
const float AntSensor::seeingRange=5.5;

Point AntSensor::Observation::getPos()const{ 
    return ent_.lock()->getPos(); 
}

Entity::Smell AntSensor::Observation::getSmell()const{
    return ent_.lock()->getSmell(); 
}

std::vector<AntSensor::Observation> AntSensor::getObservations(){
    std::vector<Observation> ret;
    
    for(const auto& a : world_->getEntityPtrs()){
        if(a.lock()->getPos().getDistance(owner_->getPos()) <= seeingRange)
            ret.push_back(Observation(a));
    }
    
    Point ownerPos=owner_->getPos();
    std::sort(ret.begin(),ret.end(),
        [ownerPos] (const Observation& a,const Observation& b) -> bool 
        { return ownerPos.getDistance(a.getPos()) 
            < ownerPos.getDistance(b.getPos()); });
    
    return ret;
}

bool AntSensor::isAccessible(const Observation& o){
    return BodyPart::isAccessible(o.getPos());
}


Point AntSensor::getClosestPheromone(PheromoneMap::Type pType, float distance){
    float range=pheromoneRange;
    int r=int(range+1);
    Point ownPos=owner_->getPos();

    Point bestFit=Point(INT_MAX,INT_MAX);

    for(const auto pm : world_->getSimulationObjects<PheromoneMap>()){
        if(pm->getType() != pType)
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

Point AntSensor::getFarthestPheromone(PheromoneMap::Type pType,float distance){
    float range=pheromoneRange;
    int r=int(range+1);
    Point ownPos=owner_->getPos();
    Point bestFit=ownPos;

    for(const auto pm : world_->getSimulationObjects<PheromoneMap>()){
        if(pm->getType() != pType)
            continue;

        for(int dx=-r; dx<=r; ++dx){
            for(int dy=-r; dy<=r; ++dy){

                int x=ownPos.posX()+dx;
                int y=ownPos.posY()+dy;
                Point pos=Point(x,y);

                if(!pos.isInBounds(world_->getDimensions())) 
                    continue;

                if(pos.getDistance(ownPos)>distance)
                    continue;

                if(pm->getStrengthAtPosition(pos)<0.1)
                    continue;

                if(pos.getDistance(ownPos) > bestFit.getDistance(ownPos)){
                    bestFit=pos;
                }
            }
        }
    }
    return bestFit;
}

float AntSensor::getPheromoneStrength(PheromoneMap::Type type, Point pos){
    for(const auto pm : world_->getSimulationObjects<PheromoneMap>()){
        if(pm->getType() != type)
            continue;
        if(owner_->getPos().getDistance(pos)<=pheromoneRange)
            return pm->getStrengthAtPosition(pos);
        else 
            return 0;
    }
    return 0;
}

Point AntSensor::findAdjecentPos(Point p){
    for(int dx=-1; dx<=1; ++dx){
        for(int dy=-1; dy<=1; ++dy){
            if((dx==0) xor (dy==0)){
                Point adjP=Point(p.posX()+dx,p.posY()+dy);
                if(BodyPart::isAccessible(adjP)){
                    return adjP;
                }
            }
        }
    }
    return Point(INT_MAX,INT_MAX);
}

// AntMandibles
boost::weak_ptr<Entity> AntMandibles::getHoldingObject() const
{
	return holdingObject_;
}

bool AntMandibles::grab(boost::weak_ptr<Entity> e){
    
    if(isHolding())
        return false;
    
    if (owner_->getPos()== e.lock()->getPos()){
        holdingObject_ = e;
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

void AntLarvaBody::step(int deltaTime){
    // TODO
    assert(0);
}
