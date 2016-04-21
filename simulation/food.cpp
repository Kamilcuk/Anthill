/*
 * Food.cpp
 *
 *  Created on: 10 mar 2016
 *      Author: kamil
 */

#include "food.hpp"


Food::Food(World* world, Point pos) :
	Entity(world, pos),
	used(false)
{
}

Food::Food(World* world) :
	Entity(world),
	used(false)
{
}

bool Food::getUsed() const
{
	return used;
}

void Food::setUsed(bool value)
{
	used = value;
}

void Food::step(int deltaTime)
{
	if ( getUsed() == true )
        this->flagToRemove();
}
