#include "anthill.hpp"
#include "world.hpp"

Anthill::Anthill(World& world,Point pos) :
	Entity(world, pos)
{

}

void Anthill::step()
{
	/** really? visitator... must be force with me! */
	std::vector<Entity *> standingon = this->getWorld().getClosestEntities(this->getPos(), 0);
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
