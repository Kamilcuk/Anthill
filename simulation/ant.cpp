/*
 * Ant.cpp
 *
 *  Created on: 10 mar 2016
 *      Author: kamil
 */

#include "ant.hpp"
#include <iostream>
#include "pheromone.hpp"



Ant::Ant(World& world, Point pos) :
	Entity(pos),
	world_(world),
	lifted_(nullptr) {

}

void Ant::lift(Entity& l) {
	lifted_ = &l;
}

Entity *Ant::unLift() {
	if ( lifted_ != nullptr ) {
		Entity *temp = lifted_;
		lifted_ = nullptr;
		return temp;
	}
	return nullptr;
}

Entity *Ant::getLifted() const {
	return lifted_;
}


void Ant::step() {
	/** how to get to other ants? */
	/** how to get to Pheromones */
	/** how to increment pos to chase them? */
	/** so many questions */
	Point goThere(-1,-1);
	Point add(-1,-1);
	int pheromoneStrengh = -1;

	std::cout << "pos_: "; this->getPos().printPosition();

	std::vector<Entity *> closest = world_.getClosestEntities(this->getPos(), this->visibility_);
	for(Entity *e : closest) {
		std::cout << "^ dupa: " << e << " ";
		e->getPos().printPosition();
	}

	/* go to food in visibility range */
	/** close to food? go for it! pick it! */
/*	for( Living *l : closest ) {
		//when food visible and no carring -> go straight to food
		if ( !this->getLifted() ) {
			Food *f = dynamic_cast<Food*>(l);
			if ( f ) {
				goThere = f->getPosition();
				break;
			}
		} else {
		}
	}*/

	int t, addX=-1, addY=-1;
/*	std::cout << " ^ GoThere: "; goThere.printPosition();
	t = this->posX() - goThere.posX();
	addX = ( t == 0 ? 0 : t < 0 ? +1 : -1 ) * speed_;

	t = this->posY() - goThere.posY();
	addY = ( t == 0 ? 0 : t < 0 ? +1 : -1 ) * speed_;

	if ( addX != 0 && addY != 0 ) {
		world_.addSimulable(new Pheromone(Position(this->getPos().posX(), this->getPos().posY())));
	}*/
	//this->move(addX, addY);

	std::cout << "^ mrowka: " << this << "addX:" << addX << "addY: " << addY << std::endl;
	std::cout << std::endl;
}
