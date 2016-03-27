#include "bodyParts.hpp"
#include "creature.hpp"
#include "world.hpp"

#include <iostream>

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

bool AntMandibles::grab(Entity* e){
    if(owner_->getPos()!=e->getPos())
        return 0;
    if(holdingObject_!=nullptr){
        return 0;
    }
    holdingObject_=e;
    return 1;
}

void AntMandibles::step(){
    if(holdingObject_!=nullptr){
        holdingObject_->setPos(owner_->getPos());
    }
}

std::vector<Entity*> AntSensor::getEntities(){
    // TODO - now don't see anything
    return std::vector<Entity*>();
}
