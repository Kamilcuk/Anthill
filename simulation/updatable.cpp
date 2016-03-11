#include "updatable.hpp"
#include "world.hpp"

Updatable::Updatable(World& world) :
	world_(world)
{
	world_.addUpdatable(this);
}

Updatable::~Updatable()
{
	world_.removeUpdatable(this);
}

World &Updatable::getWorld()
{
	return world_;
}
