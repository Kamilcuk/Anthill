#include "bodyParts.hpp"
#include "creature.hpp"
#include "world.hpp"

#include <iostream>

// Leg
void Leg::goToPos(const Point& p){
    Point curPos=owner_->getPos();
    int x=curPos.posX();
    int y=curPos.posY();
    if(curPos.posX() != p.posX())
        x+= (curPos.posX() < p.posX()) ? 1 : -1;
    if(curPos.posY() != p.posY())
        y+= (curPos.posY() < p.posY()) ? 1 : -1;
    owner_->setPos(Point(x,y));
}

// AntSensor
Point AntSensor::Observation::getPos(){ 
    return ent_.lock()->getPos(); 
}

int AntSensor::Observation::getSmell(){
    return ent_.lock()->getSmell(); 
}

std::vector<AntSensor::Observation> AntSensor::getEntities(){
    // TODO - now see everything
    std::vector<Observation> ret;
    for(auto a : world_.getDerivedUpdatable<Entity>()){
        ret.push_back(Observation( a ));
    }
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

void AntMandibles::step(){
    if(isHolding()){
        holdingObject_.lock()->setPos(owner_->getPos());
    }
}

