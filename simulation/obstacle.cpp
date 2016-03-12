#include "obstacle.hpp"

Obstacle::Obstacle(World& world, Point pos) :
	Entity(world, pos)
{
	
}

void Obstacle::step()
{
	// in future: some obstacles may move something
	// for example water pool obstacles will dry out with time
}