#include "anthill.hpp"

Anthill::Anthill(World& world,Point pos) :
	Entity(pos),
	world_(world)
{

}

void Anthill::step()
{
	/** really? visitator... must be force with me! */
	std::vector<Entity *> standingon = world_.getClosestEntities(this->getPos(), 0);
	for(Entity *e : standingon) {
		Food *f = dynamic_cast<Food*>(e);
		if ( f && !f->getCarrier() ) {
			this->lift(f);
		}
	}
}

void Anthill::lift(Food *f)
{
	if(f) {
		food_.push_back(f);
		f->addCarrier(this);
	}
}
