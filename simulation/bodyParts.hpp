#ifndef BODY_PARTS_H
#define BODY_PARTS_H

#include "updatable.hpp"
#include "point.hpp"
#include <vector>
#include <iostream>

using std::weak_ptr;
using std::shared_ptr;

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

class AntMandibles;

class AntSensor : public BodyPart{
public:
    class Observation {
        weak_ptr<Entity> ent_;
    public:
        Observation(shared_ptr<Entity> e){
            ent_=weak_ptr<Entity>( e );
        }
        Point getPos();
        int getSmell();

        // All body parts implements whole physics.
        // AntMandibles know more about this object
        // than external user (controller) and this user
        // can order AntMandibles to grab this Observation,
        // but cannot get internal Entity pointer
        friend AntMandibles;
    };
    AntSensor(World& w,Creature* owner):
        BodyPart(w,owner){}

    std::vector<Observation> getEntities();
};

class AntMandibles : public BodyPart{
    weak_ptr<Entity> holdingObject_;
public:
    AntMandibles(World& w,Creature* owner):
        BodyPart(w,owner){};
    bool grab(weak_ptr<Entity> e);
    bool grab(AntSensor::Observation o);
    void step();
    bool isHolding(){ return holdingObject_.lock() != NULL; }
};


#endif // BODY_PARTS_H
