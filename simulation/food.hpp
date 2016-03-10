/*
 * Food.h
 *
 *  Created on: 10 mar 2016
 *      Author: kamil
 */

#ifndef SRC_FOOD_HPP_
#define SRC_FOOD_HPP_

#include "entity.hpp"

class Food : public Entity {
public:
	Food(Point pos);
	void step();
};

#endif /* SRC_FOOD_HPP_ */
