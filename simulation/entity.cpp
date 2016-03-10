#include "entity.hpp"

Point &Entity::getPos()
{
	return pos_;
}

Entity::Entity(Point pos) :
	pos_(pos),
	carrier_(nullptr)
{

}

void Entity::addCarrier(Entity *e)
{
	if ( !carrier_ )
		carrier_ = e;
}

void Entity::removeCarrier()
{
	carrier_ = nullptr;
}

Entity *Entity::getCarrier() const
{
	return carrier_;
}

