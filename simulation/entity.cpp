#include "entity.hpp"

#include <boost/shared_ptr.hpp>

#include "world.hpp"
#include "entity2DMap.hpp"

Entity::Entity(World* world, Point pos) :
    Updatable(world),
    pos_(pos)
{
    hasCollision_ = false;
    smell_ = Smell::None;
}

Entity::Entity(World *world) :
    Updatable(world)
{
    hasCollision_ = false;
    smell_ = Smell::None;
}

Entity::Entity(const Entity& other) :
    Updatable(other.world_)
{
    world_ = other.world_;
    pos_ = other.pos_;
    hasCollision_ = other.hasCollision_;
    energy_ = other.energy_;
    maxEnergy_ = other.maxEnergy_;
    smell_ = other.smell_;
}

Entity::~Entity()
{
    world_->getEntityMap().lock()->invalidate();
}

Point Entity::getPos() const
{
    return this->pos_;
}

void Entity::setPos(Point pos)
{
    const Point old_pos = this->pos_;
    this->pos_ = pos;
    world_->getEntityMap().lock()->move(shared_from_this(), old_pos);
}

float Entity::getDistance(Entity * const e) const
{
    return this->getPos().getDistance(e->getPos());
}

Entity& Entity::operator=(const Entity& other)
{
    world_ = other.world_;
    pos_ = other.pos_;
    hasCollision_ = other.hasCollision_;
    energy_ = other.energy_;
    maxEnergy_ = other.maxEnergy_;
    smell_ = other.smell_;
    return *this;
}
