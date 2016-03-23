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


Ant::Ant(World& world, Point pos) :
	Creature(world, pos),
	world_(world)
{
    addLeg();
}

Ant::~Ant()
{

}

#define dupaprint(str,e) std::cout<<str" type: "<<typeid(*e).name()<<" "<<e<<"pos: "; e->getPos().printPosition();

void Ant::step() {
    auto legs=getLegs();
    if(!legs.empty())
        legs[0].goToPos(Point(rand()%20,rand()%20));
    else{
        std::cout<<"Nie mam nog\n";
    }
}
//	/** how to get to other ants? */
//	/** how to get to Pheromones */
//	/** how to increment pos to chase them? */
//	/** so many questions */
//	Point goThere(-1,-1);
//	bool foodThere = false;
//	bool anthillThere = false;
//	float closestFoodDistance = 1000.f;
//
//	/* this is bad */
//	std::vector<PheromoneMap *> pheromoneMaps = getWorld().getPheromoneMaps();
//	PheromoneMap *pheroFromFood;
//	PheromoneMap *pheroToFood;
//	for(PheromoneMap * p : pheromoneMaps) {
//		if ( p->getType() == PheromoneMap::ToFood ) {
//			pheroToFood = p;
//		}
//		if ( p->getType() == PheromoneMap::FromFood ) {
//			pheroFromFood = p;
//		}
//	}
//
//
//
//	dupaprint("Mrowka",this);
//	if ( this->getLifted() ) {
//		dupaprint("^ carrying",this->getLifted());
//	}
//
//	std::vector<Entity *> standingon = world_.getClosestEntities(this->getPos(), 0);
//
//	for(Entity *e : standingon) {
//		//dupaprint("^ standingon",e);
//		if ( !e ) return;
//
//		if ( !this->getLifted() ) {
//			// if we dont transport anything we can lift that up
//			Food *f = dynamic_cast<Food*>(e);
//			if ( f && !f->getCarrier() ) {
//				this->lift(static_cast<Entity*>(f));
//				f->addCarrier(static_cast<Entity*>(this));
//			}
//		} else {
//			// we are carrying something -> give that to anthill
//			Anthill *a = dynamic_cast<Anthill*>(e);
//			if ( a ) {
//				Entity *temp = this->unLift();
//				temp->removeCarrier();
//				Food *f = dynamic_cast<Food*>(temp);
//				if ( f ) {
//					a->lift(f);
//				}
//			}
//		}
//	}
//
//	std::vector<Entity *> closest = world_.getClosestEntities(this->getPos(), this->visibility_);
//	for(Entity *e : closest) {
//		//dupaprint("^ closest",e);
//
//		//when food visible and no carring -> go straight to closest food
//		// when no carrying and pheromene is visible -> chooste best pheromeno
//		// when carrying food -> go to anthill (randomly :D )
//		// now ! this should be a visitor!
//		if ( !this->getLifted() ) {
//			Food *f = dynamic_cast<Food*>(e);
//			if ( f && !f->getCarrier() ) {
//				float distance = this->getDistance(f);
//				if ( distance < closestFoodDistance ) {
//					std::cout << "FOOD! distance: " << distance << std::endl;
//					closestFoodDistance = distance;
//					goThere = f->getPos();
//					foodThere = true;
//				}
//			}
//		} else {
//			Anthill *a = dynamic_cast<Anthill*>(e);
//			if ( a ) {
//				goThere = a->getPos();
//				anthillThere = true;
//				break;
//			}
//		}
//	}
//
//	if ( goThere.posX() == -1 && goThere.posY() == -1 ) {
//		Point strongestPheromone(-1,-1);
//		float pheroStrength = 0;
//		if ( this->getLifted() ) {
//			strongestPheromone = pheroFromFood->getStrongestAtArea(this->getPos(), this->visibility_);
//			if ( strongestPheromone.posX() > 0 && strongestPheromone.posY() > 0 )
//				pheroStrength = pheroFromFood->getStrengthAtPosition(strongestPheromone);
//		} else {
//			strongestPheromone = pheroToFood->getStrongestAtArea(this->getPos(), this->visibility_);
//			if ( strongestPheromone.posX() > 0 && strongestPheromone.posY() > 0 )
//				pheroStrength = pheroToFood->getStrengthAtPosition(strongestPheromone);
//		}
//		if ( pheroStrength > 2 || strongestPheromone.posX() == -1 || strongestPheromone.posY() == -1 ) {
//			std::cout << " strongestPheromone " << pheroStrength << std::endl;
//			goThere = strongestPheromone;
//		} else {
//			/* aa! no where to go? go somewhere random! */
//			std::cout << " random " << std::endl;
//			goThere = this->getPos().move(rand()%2 ? -1 : 1,rand()%2 ? -1 : 1);
//		}
//	}
//	if ( goThere.posX() < 0 || goThere.posY() < 0 ) {
//		goThere =  Point(30,30);
//	}
//
//		/* calculate next position */
//		int t, addX=-1, addY=-1;
//		std::cout << " ^ GoThere: "; goThere.printPosition();
//		t = this->getPos().posX() - goThere.posX();
//		addX = ( t == 0 ? 0 : t < 0 ? +1 : -1 ) * speed_;
//		t = this->getPos().posY() - goThere.posY();
//		addY = ( t == 0 ? 0 : t < 0 ? +1 : -1 ) * speed_;
//		Point newPos = this->getPos().move(addX, addY);
//
//		/** pheromones!
//		  *  but only when we are following a path! */
//		if ( addX != 0 && addY != 0 ) {
//			if ( anthillThere && this->getLifted() ) {
//				std::cout << "pheroFromFood" << std::endl;
//				pheroFromFood->createBlob(this->getPos(), 2, 100);
//			}
//			if ( foodThere && !this->getLifted() ) {
//				std::cout << "phero to food" << std::endl;
//				pheroToFood->createBlob(this->getPos(), 2, 100);
//			}
//		}
//
//		/* set position */
//		this->setPos( newPos );
//		std::cout << "^ idz do: addX: " << addX << " addY: " << addY << std::endl;
//
//	std::cout << std::endl;
//}
