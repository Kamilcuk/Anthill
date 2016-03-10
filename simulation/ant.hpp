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

class World;
class Ant : public Entity {
	/** reference to the whole world! */
	World& world_;

	/** something lifted by ant, or null */
	Entity *lifted_;

	const int speed_ = 1;
	const int visibility_ = 3;
public:
	Ant(World& world, Point pos);

	void lift(Entity *l);
	Entity *unLift();
	Entity *getLifted() const;

	void step();
};

#endif /* ANT_H_ */
