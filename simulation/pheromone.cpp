#include "pheromone.hpp"

Pheromone::Pheromone(World& world, Pheromone::Type type, Point pos) :
	world_(world),
	Entity(pos),
	type_(type),
	strength_(10)
{

}


void Pheromone::step()
{
	/** less strenght with time */
	strength_--;

	/** is this ok? */
	if ( strength_ ) {
		//world_.remove costam jeszcze nie wiem;
		return;
	}
}
