#pragma once
#include "entity.hpp"
#include "bodyParts.hpp"

#include <vector>

class Creature : public Entity{
    // vectors of body parts
    std::vector<Leg*> legs;
    std::vector<AntMandibles*> antMandibles;
    std::vector<AntSensor*> antSensors;

protected:
    void addLeg(){
        legs.push_back(new Leg(world_,this));
    }
    void addAntMandibles(){
        antMandibles.push_back(new AntMandibles(world_,this));
    }
    void addAntSensor(){
        antSensors.push_back(new AntSensor(world_,this));
    }
	Creature(World& world, Point pos):
        Entity(world,pos){}

public:
    std::vector<Leg*>& getLegs(){
        return legs;
    }
    std::vector<AntMandibles*>& getAntMandibles(){
        return antMandibles;
    }
    std::vector<AntSensor*>& getAntSensors(){
        return antSensors;
    }
};
