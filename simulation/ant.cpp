/*
 * Ant.cpp
 *
 *  Created on: 10 mar 2016
 *      Author: kamil
 */

#include "ant.hpp"
#include <iostream>
#include <cstdlib>
#include "anthill.hpp"
#include "pheromone.hpp"


Ant::Ant(World& world, Point pos) :
	Entity(world, pos),
	world_(world)
{

}

#define dupaprint(str,e) std::cout<<str" type: "<<typeid(*e).name()<<" "<<e<<"pos: "; e->getPos().printPosition();

void Ant::step() {
	/** how to get to other ants? */
	/** how to get to Pheromones */
	/** how to increment pos to chase them? */
	/** so many questions */
	Point goThere(-1,-1);
	int mostStrenghPheromone = -1;

	dupaprint("Mrowka",this);
	if ( this->getLifted() ) {
		dupaprint("^ carrying",this->getLifted());
	}

	std::vector<Entity *> standingon = world_.getClosestEntities(this->getPos(), 0);

	for(Entity *e : standingon) {
		dupaprint("^ standingon",e);

		if ( !this->getLifted() ) {
			// if we dont transport anything we can lift that up
			Food *f = dynamic_cast<Food*>(e);
			if ( f && !f->getCarrier() ) {
				this->lift(static_cast<Entity*>(f));
				f->addCarrier(static_cast<Entity*>(this));
			}
		} else {
			// we are carrying something -> pull that down on anthill
			Anthill *a = dynamic_cast<Anthill*>(e);
			if ( a ) {
				Entity *temp = this->unLift();
				temp->removeCarrier();
			}
		}
	}

	std::vector<Entity *> closest = world_.getClosestEntities(this->getPos(), this->visibility_);
	for(Entity *e : closest) {
		dupaprint("^ closest",e);

		//when food visible and no carring -> go straight to food
		// when no carrying and pheromene is visible -> chooste best pheromeno
		// when carrying food -> go to anthill (randomly :D )
		// now ! this should be a visitor!
		if ( !this->getLifted() ) {
			Food *f = dynamic_cast<Food*>(e);
			if ( f && !f->getCarrier() ) {
				goThere = f->getPos();
				break;
			}
			Pheromone *p = dynamic_cast<Pheromone*>(e);
			if ( p && mostStrenghPheromone < p->getStrength() ) {
				goThere = p->getPos();
			}
		} else {
			Anthill *a = dynamic_cast<Anthill*>(e);
			if ( a ) {
				goThere = a->getPos();
			}
		}
	}

	if ( goThere.posX() == -1 && goThere.posY() == -1 ) {
		/* aa! no where to go? go somewhere random! */
		this->getPos().move(rand()%2-1 ? -1 : 1,rand()%3-1 ? -1 : 1);
	} else {
		int t, addX=-1, addY=-1;
		std::cout << " ^ GoThere: "; goThere.printPosition();

		t = this->getPos().posX() - goThere.posX();
		addX = ( t == 0 ? 0 : t < 0 ? +1 : -1 ) * speed_;
		t = this->getPos().posY() - goThere.posY();
		addY = ( t == 0 ? 0 : t < 0 ? +1 : -1 ) * speed_;

		if ( addX != 0 && addY != 0 ) {
			//world_.addSimulable(new Pheromone(Position(this->getPos().posX(), this->getPos().posY())));
		}
		this->setPos( this->getPos().move(addX, addY) );
		std::cout << "^ idz do: addX: " << addX << " addY: " << addY << std::endl;
	}

	std::cout << std::endl;
}
