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
#include "visitable.hpp"

class World;
class Ant : public Creature, public Visitable {
	const int speed_ = 1;
	const int visibility_ = 15;

public:
	Ant(World& world, Point pos);
	~Ant();

	void step(int);

    // Visitable interface
    void accept(Visitor& v) const;
};

#endif /* ANT_H_ */
