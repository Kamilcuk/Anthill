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
	bool used;
public:
	Food(World* world, Point pos);

	void step(int);
	bool getUsed() const;
	void setUsed(bool value);

    int getSmell(){return 100;}
};

#endif /* SRC_FOOD_HPP_ */
