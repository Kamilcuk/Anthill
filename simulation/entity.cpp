#include "entity.hpp"
#include "world.hpp"

Entity::Entity(World* world, Point pos) :
    Updatable(world),
    pos_(pos)
{
}

Entity::Entity(World *world) :
    Updatable(world)
{
}

Entity::Entity(const Entity& other) :
    Updatable(other.world_),
    pos_(other.pos_)
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

float Entity::getDistance(Entity * const e)const
{
    return this->getPos().getDistance(e->getPos());
}

Entity& Entity::operator=(const Entity& other)
{
    world_ = other.world_;
    pos_ = other.pos_;
    return *this;
}