#include "obstacle.hpp"

#include <iostream>

Obstacle::Obstacle(World* world, Point pos) :
	Visitable(world),
	Entity(world, pos)
{
    smell_=Smell::None;
    energy_=20;
    maxEnergy_=20;
	hasCollision_ = true;
}

Obstacle::Obstacle(World* world) :
	Visitable(world),
	Entity(world)
{
	hasCollision_ = true;
}

void Obstacle::step(int)
{
	// in future: some obstacles may move something
	// for example water pool obstacles will dry out with time
    
    if (energy_<=0){
        flagToRemove();
    }
}

float Obstacle::bite(float strength){
    energy_-=strength;
    return 0;
}

void Obstacle::accept(Visitor &v) const
{
	v.visit(*this);
}
