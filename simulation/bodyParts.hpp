#ifndef BODY_PARTS_H
#define BODY_PARTS_H

#include "updatable.hpp"
#include "point.hpp"

//#include "creature.hpp"
class Creature;


class BodyPart : public Updatable{
protected:
    Creature* owner_;
public:
    BodyPart(World& w,Creature* c):
        Updatable(w),owner_(c){}
};

class Leg : public BodyPart{
public:
    Leg(World& w,Creature* owner):
        BodyPart(w,owner){}
    void step(){
    }
    void goToPos(const Point& p);
};

#endif // BODY_PARTS_H
