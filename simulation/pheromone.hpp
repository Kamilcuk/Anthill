#ifndef PHEROMONE_H
#define PHEROMONE_H

#include "position.hpp"
#include "living.hpp"
#include "world.hpp"

class Pheromone : public Living
{
	enum Type {
		toFood,
		fromFood,
	};

	World& world_;
	Type type_;

	int strength_;


public:
	Pheromone(World& world, Pheromone::Type type, Position pos);
	void step();
};

#endif // PHEROMONE_H
