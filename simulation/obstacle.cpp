#include "obstacle.hpp"

#include <iostream>

Obstacle::Obstacle(World* world, Point pos) :
	Entity(world, pos)
{
}

void Obstacle::step(int)
{
	// in future: some obstacles may move something
	// for example water pool obstacles will dry out with time
}
