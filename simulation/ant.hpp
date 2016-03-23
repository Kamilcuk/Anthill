/*
 * Ant.h
 *
 *  Created on: 10 mar 2016
 *      Author: kamil
 */

#ifndef ANT_H_
#define ANT_H_

#include "entity.hpp"
#include "world.hpp"
#include "creature.hpp"

class World;
class Ant : public Creature {
	/** reference to the whole world! */
	World& world_;

	const int speed_ = 1;
	const int visibility_ = 15;
public:
	Ant(World& world, Point pos);
	~Ant();

	void step();
};

#endif /* ANT_H_ */
