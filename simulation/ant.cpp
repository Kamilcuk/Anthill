/*
 * Ant.cpp
 *
 *  Created on: 10 mar 2016
 *      Author: kamil
 */

#include "ant.hpp"


int Ant::getSpeed() const
{
    return speed_;
}

Ant::Ant(World& world, Point pos, Controller *controller) :
    Entity(world, pos),
    controller_(controller)
{
    this->setVisibilityRadius(15);
}

Ant::~Ant()
{

}

void Ant::step() {
    controller_->control(this);
}


