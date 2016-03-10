#ifndef PHEROMONE_H
#define PHEROMONE_H

#include "point.hpp"
#include "entity.hpp"
#include "world.hpp"

class Pheromone : public Entity
{
	enum Type {
		toFood,
		fromFood,
	};

	World& world_;
	Type type_;

	int strength_;


public:
	Pheromone(World& world, Pheromone::Type type, Point pos);
	void step();
};

#endif // PHEROMONE_H
