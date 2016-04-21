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

Updatable& Updatable::operator=(const Updatable& other)
{
    world_ = other.world_;
    return *this;
}

bool Updatable::operator==(const Updatable& other)
{
    return *this == other;
}

bool Updatable::operator!=(const Updatable& other)
{
    return *this != other;
}