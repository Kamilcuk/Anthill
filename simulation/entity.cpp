#include "entity.hpp"

Point Entity::getPos()
{
	return pos_;
}

Entity::Entity(World& world, Point pos) :
	Updatable(world),
	pos_(pos),
	carrier_(nullptr),
	lifted_(nullptr)
{

}

Entity::~Entity()
{

}

void Entity::setPos(Point pos)
{
	this->pos_ = pos;
}

void Entity::addCarrier(Entity *e)
{
	if ( !carrier_ ) {
		this->getPos() = e->getPos();
		carrier_ = e;
	}
}

Entity *Entity::removeCarrier()
{
	if ( carrier_ == nullptr ) {
		return nullptr;
	}
	this->getPos() = Point(carrier_->getPos());

	Entity *temp = carrier_;
	carrier_ = nullptr;
	return temp;
}

Entity *Entity::getCarrier() const
{
	return carrier_;
}


void Entity::lift(Entity *l) {
	lifted_ = l;
}

Entity *Entity::unLift() {
	if ( lifted_ != nullptr ) {
		Entity *temp = lifted_;
		lifted_ = nullptr;
		return temp;
	}
	return nullptr;
}

Entity *Entity::getLifted() const {
	return lifted_;
}
