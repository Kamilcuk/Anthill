/*
 * Food.cpp
 *
 *  Created on: 10 mar 2016
 *      Author: kamil
 */

#include "food.hpp"


bool Food::getUsed() const
{
	return used;
}

void Food::setUsed(bool value)
{
	used = value;
}

Food::Food(World& world, Point pos) :
	Entity(world, pos),
	used(false)
{

}

void Food::step()
{
	if ( getUsed() == true )
		delete this;
}
