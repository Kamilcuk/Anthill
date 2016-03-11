/*
 * World.cpp
 *
 *  Created on: 9 mar 2016
 *      Author: ms
 */

#include "world.hpp"
#include "anthill.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
using std::vector;

std::vector<Updatable *> World::getUpdatables() const
{
	return updatables_;
}

World::World() :
	pheromoneMap_(*this, 300, 200)
{
	setDimensions(300, 200);
}
World::~World(){
}

//boost::python::list World::getAntsPythonList(){
//    return std_vector_to_py_list(ants);
//}

void World::setDimensions(int X, int Y)
{
	width = X;
	height = Y;
}
void World::setSimulationFramerate(float){
}
void World::startSimulation()
{
	/* initialize random seed: */
	srand (time(NULL));
	new Ant(*this, Point(1,1));
	new Ant(*this, Point(100,100));
	new Food(*this, Point(30,30));
	new Food(*this, Point(70,70));
	new Anthill(*this, Point(50,50));
}

void World::stopSimulation()
{
	for(Updatable *s : updatables_) {
		delete s;
	}
	updatables_.clear();
}

void World::simulationStep()
{
	std::cout<<"--------------------------------------------"<<std::endl;
	for(Updatable *s : updatables_) {
		(*s).step();
	}
	pheromoneMap_.step();
}


std::vector<Ant *> World::getAnts()
{
	std::vector<Ant*> ants;
	for(Updatable *s : updatables_) {
		Ant *a = dynamic_cast<Ant*>(s);
		if( a ) {
			ants.push_back(a);
		}
	}
	return ants;
}

std::vector<Food *> World::getFoods()
{
	std::vector<Food*> foods;
	for(Updatable *s : updatables_) {
		Food *a = dynamic_cast<Food*>(s);
		if( a ) {
			foods.push_back(a);
		}
	}
	return foods;
}

std::vector<Entity *> World::getClosestEntities(Point mypos, int visibility)
{
	std::vector<Entity*> ret;
	for(Updatable *u : updatables_) {
		Entity *e = dynamic_cast<Entity*>(u);
		if ( !e )
			continue;
		if ( abs(mypos.posX()-e->getPos().posX()) <= visibility &&
				abs(mypos.posY()-e->getPos().posY()) <= visibility ) {
			ret.push_back(e);
		}
	}
	return ret;
}

void World::addUpdatable(Updatable *e)
{
	if ( e )
		updatables_.push_back(e);
}

void World::removeUpdatable(Updatable *e)
{
	std::vector<Updatable*>::iterator it;
	for(it = updatables_.begin(); it != this->updatables_.end(); ++it) {
		if ( *it == e ) {
			updatables_.erase(it);
			delete *it;
			return;
		}
	}
}

PheromoneMap &World::getPheromoneMap()
{
	return pheromoneMap_;
}
