#include "entity.hpp"
#include "world.hpp"
#include "food.hpp"
#include <algorithm>
#include "anthill.hpp"


Entity::Entity(World& world, Point pos) :
    Updatable(world),
    pos_(pos)
{
}

Entity::~Entity()
{

}

Point Entity::getPos() const
{
    return this->pos_;
}


void Entity::setPos(Point pos)
{
    this->pos_ = pos;
}


float Entity::getDistance(Entity * const e)
{
    return this->getPos().getDistance(e->getPos());
}
