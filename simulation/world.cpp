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

World::World()
{
	setDimensions(300, 200);

	/* initialize random seed: */
	srand (time(NULL));

}

World::~World()
{
	this->stopSimulation();
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
	/* they add itself*/
	/* remember one pheromone map per world! */
	new PheromoneMap(*this, width, height);

	new Ant(*this, Point(1,1));
	new Ant(*this, Point(100,100));
	new Food(*this, Point(30,30));
	new Food(*this, Point(70,70));
	new Anthill(*this, Point(50,50));
}

void World::stopSimulation()
{
	// as Updatable removes itself from the list
	// we need a copy of the list to operate on
	std::vector<Updatable *> updatables2_ = updatables_;
	for(Updatable *s : updatables2_) {
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
	if ( e && std::find(updatables_.begin(), updatables_.end(), e) == updatables_.end() )
		updatables_.push_back(e);
}

void World::removeUpdatable(Updatable *e)
{
	std::vector<Updatable*>::iterator it;
	it = std::find(updatables_.begin(), updatables_.end(), e);
	if ( it == updatables_.end() ) {
		// no such elemnt
		return;
	}
	updatables_.erase(it);
}

std::vector<Anthill*> World::getAnthills()
{
	/* to pownnien być szablon!!! */
	std::vector<Anthill*> tmp;
	for(Updatable *s : updatables_) {
		Anthill *a = dynamic_cast<Anthill*>(s);
		if( a ) {
			tmp.push_back(a);
		}
	}
	return tmp;
}

PheromoneMap *World::getPheromoneMap()
{
	/* to pownnien być szablon!!! */
	std::vector<PheromoneMap*> tmp;
	for(Updatable *s : updatables_) {
		PheromoneMap *a = dynamic_cast<PheromoneMap*>(s);
		if( a ) {
			return a;
		}
	}
	return nullptr;
}
