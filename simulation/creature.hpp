#pragma once
#include "entity.hpp"
#include "bodyParts.hpp"

#include <vector>

class Creature : public Entity{
    // vectors of body parts
    std::vector<shared_ptr<Leg> > legs;
    std::vector<shared_ptr<AntMandibles> > antMandibles;
    std::vector<shared_ptr<AntSensor> > antSensors;

protected:
    void addLeg();
    void addAntMandibles();
    void addAntSensor();
	Creature(World& world, Point pos):
        Entity(world,pos){}

public:
    std::vector<shared_ptr<Leg> >& getLegs(){
        return legs;
    }
    std::vector<shared_ptr<AntMandibles> >& getAntMandibles(){
        return antMandibles;
    }
    std::vector<shared_ptr<AntSensor> >& getAntSensors(){
        return antSensors;
    }
};
