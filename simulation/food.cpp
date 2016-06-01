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
    energy_=100;
	smell_ = Smell::Food;
}

Food::Food(World* world) :
	Visitable(world),
	Entity(world),
	used(false)
{
	smell_ = Smell::Food;
}

bool Food::getUsed() const
{
	return used;
}

void Food::setUsed(bool value)
{
	used = value;
}

float Food::bite(float strength){
    float ret=std::min(energy_,strength);
    energy_-=strength;
    return ret;
}

void Food::accept(Visitor &v) const
{
	v.visit(*this);
}

void Food::step(int deltaTime)
{
    if (energy_<=0){
        flagToRemove();
    }
}
