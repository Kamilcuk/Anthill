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
#include "controller.h"
#include "antcontroller.h"

class Ant : public Entity {
    Controller *controller_;

    const int speed_ = 1;
public:
    Ant(World& world, Point pos, Controller *controller = new AntController());
	~Ant();

	void step();
    int getSpeed() const;
};

#endif /* ANT_H_ */
