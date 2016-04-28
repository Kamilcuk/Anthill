#include "obstacle.hpp"

#include <iostream>

Obstacle::Obstacle(World* world, Point pos) :
	Visitable(world),
	Entity(world, pos)
{
}

Obstacle::Obstacle(World* world) :
	Visitable(world),
	Entity(world)
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
