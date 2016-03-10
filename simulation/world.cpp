/*
 * World.cpp
 *
 *  Created on: 9 mar 2016
 *      Author: ms
 */

#include "world.hpp"
#include <iostream>;
using std::vector;

World::World(){
}
World::~World(){
}

boost::python::list World::getAntsPythonList(){
    return std_vector_to_py_list(ants);
}

void World::setDimensions(int,int){
}
void World::setSimulationFramerate(float){
}
void World::startSimulation()
{
	Ant *a1 = new Ant(*this, Point(1,1));
	entities_.push_back(a1);

	Ant *a2 = new Ant(*this, Point(10,10));
	entities_.push_back(a2);

	//Food *f1 = new Food(Point(3,3));
	//entities_.push_back(f1);
}

void World::stopSimulation()
{
	for(Entity *s : entities_) {
		delete s;
	}
	entities_.clear();
}

void World::simulationStep()
{
	for(Entity *s : entities_) {
		(*s).step();
	}
}


std::vector<Ant *> World::getAnts()
{
	std::vector<Ant*> ants;
	for(Entity *s : entities_) {
		Ant *a = static_cast<Ant*>(s);
		if( a ) {
			ants.push_back(a);
		}
	}
	return ants;
}

std::vector<Food *> World::getFoods()
{
	std::vector<Food*> foods;
	for(Entity *s : entities_) {
		Food *a = static_cast<Food*>(s);
		if( a ) {
			foods.push_back(a);
		}
	}
	return foods;
}

std::vector<Entity *> World::getClosestEntities(Point mypos, int visibility)
{
	std::vector<Entity*> ret;
	for(Entity *s : entities_) {
		if ( abs(mypos.posX()-s->getPos().posX()) < visibility &&
				abs(mypos.posY()-s->getPos().posY()) < visibility ) {
			ret.push_back(s);
		}
	}
	return ret;
}

void World::addLiving(Entity*s)
{
	if ( s )
		entities_.push_back(s);
}

void World::removeLiving(Entity *l)
{
	/* not implemented yet! */
}
