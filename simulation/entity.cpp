#include "entity.hpp"
#include "world.hpp"
#include "food.hpp"
#include <algorithm>
#include "anthill.hpp"
float Entity::getVisibilityRadius() const
{
    return visibilityRadius_;
}

void Entity::setVisibilityRadius(float visibilityRadius)
{
    visibilityRadius_ = visibilityRadius;
}

Entity::Entity(World& world, Point pos) :
    Updatable(world),
    pos_(pos)
{
    this->carrier_.reset();
    this->lifted_.reset();
}

Entity::~Entity()
{

}

Point Entity::getPos() const
{
    /* when we are carried, then out pos is equal to the thing that lifts us */
    if ( auto carrier = this->getCarrier().lock() ) {
        return carrier->getPos();
	}
    return this->pos_;
}


void Entity::setPos(Point pos)
{
    this->pos_ = pos;
}

void Entity::addCarrier(std::weak_ptr<Entity> e)
{
    if ( isCarried() ) {
        return;
    }
    carrier_ = e;
}

void Entity::removeCarrier()
{
    if ( !isCarried() ) {
        return;
    }
    auto carrier = this->getCarrier().lock();
    if ( carrier ) {
        this->setPos( carrier->getPos() );
        this->getCarrier().reset();
    }
}

std::weak_ptr<Entity> Entity::getCarrier() const
{
    return carrier_;
}

bool Entity::isCarried() const
{
    return !carrier_.expired();
}


void Entity::lift(std::weak_ptr<Entity> l) {
    if ( isLifting() ) {
        return;
    }
	lifted_ = l;
    auto lifted = lifted_.lock();
    lifted->addCarrier(std::dynamic_pointer_cast<Entity>(dynamic_cast<Updatable*>(this)->shared_from_this()));
}

void Entity::unLift() {
    if ( !isLifting() ) {
        return;
    }
    if ( auto lifted = lifted_.lock() ) {
        lifted->removeCarrier();
        lifted_.reset();
    }
}

std::weak_ptr<Entity> Entity::getLifted() const {
    return lifted_;
}

bool Entity::isLifting() const
{
    return !lifted_.expired();
}

float Entity::getDistance(Entity * const e)
{
    return this->getPos().getDistance(e->getPos());
}

template<typename Derived>
std::map<std::shared_ptr<Derived>, float> Entity::getVisibleWithDistances()
{
    std::map<std::shared_ptr<Derived>, float> ret;
    std::vector<std::shared_ptr<Derived>> entities = this->getWorld().getDerivedUpdatable<Derived>();
    for(auto& e : entities) {
        float dist = this->getDistance(e.get()); //this->getPos().getDistance(e.get()->getPos());
        if ( dist < this->getVisibilityRadius() ) {
            ret[e] = dist;
        }
    }
    return ret;
}
template std::map<std::shared_ptr<Food>, float> Entity::getVisibleWithDistances<Food>();
template std::map<std::shared_ptr<Anthill>, float> Entity::getVisibleWithDistances<Anthill>();
