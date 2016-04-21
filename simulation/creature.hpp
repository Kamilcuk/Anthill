#ifndef CREATURE_H
#define CREATURE_H

#include <vector>

#include "entity.hpp"
#include "bodyParts.hpp"

class Creature : public Entity{
public:
	Creature(World* world, Point pos):
        Entity(world, pos) {}
};

#endif // CREATURE_H