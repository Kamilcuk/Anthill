/*
 * TODO: many files, 
 * but there would be too many of them 
 * if as many as classes
 */

#ifndef BODY_PARTS_H
#define BODY_PARTS_H

#include <iostream>
#include <vector>

#include "updatable.hpp"
#include "point.hpp"
#include "pheromoneMap.hpp"

class Creature;
class Entity;


class BodyPart : public Updatable{
protected:
    Creature* owner_;
public:
    BodyPart(World* w,Creature* c):
        Updatable(w),owner_(c){}
    virtual void step(int deltaTime){};
};

class AntLegs : public BodyPart{
    Point targetPos_;
public:
    AntLegs(World* w,Creature* owner);

    void goToPos(const Point& p);
    void step(int);
};

class AntMandibles;

class AntSensor : public BodyPart{
public:
    class Observation {
        Entity* ent_;
    public:
        Observation(Entity* e) : ent_(e)
        { }
        Point getPos()const;
        int getSmell()const;

        // All body parts implements whole physics.
        // AntMandibles must know more about this object
        // than external user (controller) and this user
        // can order AntMandibles to grab this Observation,
        // but cannot get internal Entity pointer
        // so,
        friend AntMandibles;
    };
    AntSensor(World* w,Creature* owner):
        BodyPart(w,owner){}

    std::vector<Observation> getEntities();
};

class AntMandibles : public BodyPart{
    Entity* holdingObject_;
public:
    AntMandibles(World* w,Creature* owner):
       BodyPart(w,owner),
        holdingObject_(nullptr)
    {}
    bool grab(Entity* e);
    bool grab(AntSensor::Observation o);
    void step(int);
    bool isHolding(){ return holdingObject_ != nullptr; }
};

class AntWorkerAbdomen : public BodyPart{
    PheromoneMap::Type dropType;
public:
    AntWorkerAbdomen(World* w,Creature* owner):
        BodyPart(w,owner),
        dropType(PheromoneMap::Type::None) // don't drop
    {};
    void dropToFoodPheromones();
    void dropFromFoodPheromones();
    void step(int);
};


#endif // BODY_PARTS_H
