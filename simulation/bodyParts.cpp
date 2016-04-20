#include "bodyParts.hpp"
#include "creature.hpp"
#include "world.hpp"
#include "pheromoneMap.hpp"
#include "ant.hpp"

#include <iostream>

// AntLegs
AntLegs::AntLegs(World& w,Creature* owner)
    : BodyPart(w,owner){
        targetPos_=owner->getPos();
}

void AntLegs::goToPos(const Point& p){
    targetPos_=p;
}

void AntLegs::step(int deltatime){
    while((deltatime--)>0){
        Point curPos=owner_->getPos();
        int x=curPos.posX();
        int y=curPos.posY();
        if(curPos.posX() != targetPos_.posX())
            x+= (curPos.posX() < targetPos_.posX()) ? 1 : -1;
        if(curPos.posY() != targetPos_.posY())
            y+= (curPos.posY() < targetPos_.posY()) ? 1 : -1;

        // check if this position is free (there is no other Ant)
        // (detect collision)
        for(auto a : world_.getDerivedUpdatable<Ant>()){
            if(a->getPos()==Point(x,y)){
                // collision detected
                return;
            }
        }
        owner_->setPos(Point(x,y));
    }
}

// AntSensor
Point AntSensor::Observation::getPos()const{ 
    return ent_.lock()->getPos(); 
}

int AntSensor::Observation::getSmell()const{
    return ent_.lock()->getSmell(); 
}

std::vector<AntSensor::Observation> AntSensor::getEntities(){
    std::vector<Observation> ret;
    for(auto a : world_.getDerivedUpdatable<Entity>()){
        if(a->getPos().getDistance(owner_->getPos()) <= 4)
            ret.push_back(Observation( a ));
    }
    Point ownerPos=owner_->getPos();
    std::sort(ret.begin(),ret.end(),
        [ownerPos](const Observation& a,const Observation& b) -> bool {
            return ownerPos.getDistance(a.getPos()) 
                < ownerPos.getDistance(b.getPos()); 
        });
    return ret;
}

// AntMandibles
bool AntMandibles::grab(weak_ptr<Entity> e){
    auto ent=e.lock();
    if(owner_->getPos()!=ent->getPos())
        return 0;
    if(isHolding()){
        return 0;
    }
    holdingObject_=e;
    return 1;
}

bool AntMandibles::grab(AntSensor::Observation o){
    grab(o.ent_);
    return 1;
}

void AntMandibles::step(int deltaTime){
    if(isHolding()){
        holdingObject_.lock()->setPos(owner_->getPos());
    }
}

// AntWorkerAbdomen
void AntWorkerAbdomen::dropToFoodPheromones(){
    dropType=PheromoneMap::Type::ToFood;
}

void AntWorkerAbdomen::step(int deltaTime){
    if(deltaTime<=0)
        return;
    if(dropType == PheromoneMap::Type::None)
        return;
    for(auto pm : world_.getDerivedUpdatable<PheromoneMap>()){
        if(pm->getType()==dropType){
            pm->createBlob(owner_->getPos(), 2, 100);
            return;
        }
    }

    dropType = PheromoneMap::Type::None;
}


