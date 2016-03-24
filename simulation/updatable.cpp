#include "updatable.hpp"
#include "world.hpp"
#include <iostream>

Updatable::Updatable(World& world) :
	world_(world)
{
	world_.addUpdatable(this);
    std::cout<<"updatable"<<std::endl;
}

Updatable::~Updatable()
{
	world_.removeUpdatable(this);
}

World &Updatable::getWorld()
{
	return world_;
}
