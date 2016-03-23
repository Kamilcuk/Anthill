#include "bodyParts.hpp"
#include "creature.hpp"

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
