/*
 * Food.cpp
 *
 *  Created on: 10 mar 2016
 *      Author: kamil
 */

#include "food.hpp"


Food::Food(World* world, Point pos) :
	Visitable(world),
	Entity(world, pos),
	used(false)
{
}

Food::Food(World* world) :
	Visitable(world),
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

void Food::accept(Visitor &v) const
{
	v.visit(*this);
}

void Food::step(int deltaTime)
{
	if ( getUsed() == true )
        this->flagToRemove();
}
