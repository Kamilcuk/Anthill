#ifndef ANTHILL_H
#define ANTHILL_H

#include <vector>
#include "entity.hpp"
#include "food.hpp"

class World;
class Anthill : public Entity
{
	/** all the food we have! hahahaha */
	std::vector<Food *> food_;
public:
	Anthill(World& world, Point pos);
	~Anthill();
	void step();

	void lift(Food *l);
};

#endif // ANTHILL_H
