#include "obstacle.hpp"

#include <iostream>

Obstacle::Obstacle(World* world, Point pos) :
	Entity(world, pos),
	Visitable(world)
{
}

Obstacle::Obstacle(World* world) :
	Entity(world),
	Visitable(world)
{
}

void Obstacle::step(int)
{
	// in future: some obstacles may move something
	// for example water pool obstacles will dry out with time
}

void Obstacle::accept(Visitor &v) const
{
	v.visit(*this);
}
