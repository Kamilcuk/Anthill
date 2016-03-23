#pragma once
#include "entity.hpp"
#include "bodyParts.hpp"

#include <vector>

class Creature : public Entity{
    // vectors of body parts
    std::vector<Leg> legs;

protected:
    void addLeg(){
        legs.push_back(Leg(world_,this));
    }

public:
	Creature(World& world, Point pos):
        Entity(world,pos){}
    const std::vector<Leg>& getLegs(){
        return legs;
    }
};
