#include "updatable.hpp"
#include "world.hpp"

Updatable::Updatable(World &world) :
    world_(world)
{
}

Updatable::~Updatable()
{

}

World &Updatable::getWorld()
{
    return world_;
}

void Updatable::remove()
{
    alive = false;
}

bool Updatable::isAlive() const
{
    return alive;
}
