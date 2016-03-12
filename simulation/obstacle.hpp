#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "entity.hpp"

class Obstacle : public Entity
{

public:
	Obstacle(World& world, Point pos);
	void step();
};

#endif // OBSTACLE_H