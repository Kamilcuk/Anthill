#include "entity.hpp"
#include <cmath>


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

Point Entity::getPos()
{
	/* when we are carried, then out pos is equal to the thing that lifts us */
	if ( this->getCarrier() ) {
		this->pos_ = this->getCarrier()->getPos();
	}
	return this->pos_;
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

float Entity::getDistance(Entity *e)
{
	return std::sqrt(
				std::pow(this->getPos().posX()-e->getPos().posX(), 2) +
				std::pow(this->getPos().posY()-e->getPos().posY(), 2)
				);
}
