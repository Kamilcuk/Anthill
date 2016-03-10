#ifndef ANTHILL_H
#define ANTHILL_H

#include <vector>
#include "entity.hpp"
#include "food.hpp"
#include "world.hpp"

class Anthill : public Entity
{
	World& world_;

	/** all the food we have! hahahaha */
	std::vector<Food *> food_;
public:
	Anthill(World& world, Point pos);
	void step();

	void lift(Food *l);
};

#endif // ANTHILL_H
