#include "entity.hpp"
#include "world.hpp"

Entity::Entity(World* world, Point pos) :
    Updatable(world),
    pos_(pos)
{
    world_->addEntity(this);
}

Entity::~Entity()
{
    world_->removeEntity(this);
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
