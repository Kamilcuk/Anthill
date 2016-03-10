#include "pheromone.hpp"

int Pheromone::getStrength() const
{
	return strength_;
}

Pheromone::Pheromone(World& world, Pheromone::Type type, Point pos) :
	Entity(pos),
	world_(world),
	type_(type),
	strength_(10)
{

}


void Pheromone::step()
{
	/** less strenght with time */
	strength_--;

	/** is this ok? */
	if ( strength_ <= 0 ) {
		world_.removeEntity(this);
		return;
	}
}
