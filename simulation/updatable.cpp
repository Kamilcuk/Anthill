#include "updatable.hpp"
#include "world.hpp"

Updatable::Updatable(World* world) :
    world_(world)
{
    world_->addUpdatable(this);
}

Updatable::~Updatable()
{
    world_->removeUpdatable(this);
}

Updatable::Updatable(const Updatable& other)  :
    world_(other.world_)
{
    world_->addUpdatable(this);
}