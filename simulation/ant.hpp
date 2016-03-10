/*
 * Ant.h
 *
 *  Created on: 10 mar 2016
 *      Author: kamil
 */

#ifndef ANT_H_
#define ANT_H_

#include "living.hpp"
#include "world.hpp"

class World;
class Ant : public Living {
	/** reference to the whole world! */
	World& world_;

	/** lifted by ant, or null */
	Living *lifted_;

	const int speed_ = 1;
	const int visibility_ = 3;
public:
	Ant(World& world, Position pos);

	void lift(Living& l);
	Living *unLift();
	Living *getLifted() const;

	void step();
};

#endif /* ANT_H_ */
