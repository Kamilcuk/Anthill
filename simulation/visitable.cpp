#include "visitable.hpp"

#include "world.hpp"

#include <iostream>

Visitable::Visitable(World* world) :
    world_(world)
{
    world_->addVisitable(this);
}

Visitable::~Visitable()
{
    world_->removeVisitable(this);
}

Visitable& Visitable::operator=(const Visitable& other)
{
    world_ = other.world_;
    return *this;
}