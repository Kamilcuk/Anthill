#ifndef BODY_PARTS_H
#define BODY_PARTS_H

#include "updatable.hpp"
#include "point.hpp"
#include <vector>
#include <iostream>


//#include "creature.hpp"
class Creature;
class Entity;


class BodyPart : public Updatable{
protected:
    Creature* owner_;
public:
    BodyPart(World& w,Creature* c):
        Updatable(w),owner_(c){}
    virtual void step(){};
};

class Leg : public BodyPart{
public:
    Leg(World& w,Creature* owner):
        BodyPart(w,owner){}

    void goToPos(const Point& p);
};

class AntMandibles : public BodyPart{
    Entity* holdingObject_;
public:
    AntMandibles(World& w,Creature* owner):
        BodyPart(w,owner),holdingObject_(nullptr){}

    bool grab(Entity* e);
    void step();
    bool isHolding(){ return holdingObject_!=nullptr; }
};

class AntSensor : public BodyPart{
public:
    AntSensor(World& w,Creature* owner):
        BodyPart(w,owner){}

    std::vector<Entity*> getEntities();
};

#endif // BODY_PARTS_H
