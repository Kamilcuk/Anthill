#include "anthill.hpp"
#include "world.hpp"

Anthill::Anthill(World& world,Point pos) :
	Entity(world, pos)
{

}

Anthill::~Anthill()
{
	/* dont delete food_ vector, couse it gets deleted in world */
	this->food_.clear();
}

void Anthill::step()
{
	std::vector<Entity *> standingon = this->getWorld().getClosestEntities(this->getPos(), 0);
	for(Entity *e : standingon) {
		Food *f = dynamic_cast<Food*>(e);
		if ( f && !f->getCarrier() ) {
			this->lift(f);
		}
	}

	if ( food_.size() > 10 ) {
		for(auto *f : food_) {
			f->setUsed(true);
		}
		new Ant(this->getWorld(), this->getPos().move(1,1));
		food_.clear();
	}
}

void Anthill::lift(Food *f)
{
	if(f) {
		food_.push_back(f);
		f->addCarrier(this);
	}
}
