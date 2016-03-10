#include "entity.hpp"

Point Entity::getPos() const
{
	return pos_;
}

Entity::Entity(Point pos) :
	pos_(pos)
{

}
