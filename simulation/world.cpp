/*
 * World.cpp
 *
 *  Created on: 9 mar 2016
 *      Author: ms
 */

#include "world.hpp"
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
	Ant *a1 = new Ant(*this, Position(1,1));
	livings_.push_back(a1);

	//Ant *a2 = new Ant(*this, Position(10,10));
	//simulables_.push_back(a2);

	Food *f1 = new Food(Position(3,3));
	livings_.push_back(f1);
}

void World::stopSimulation()
{
	for(Living *s : livings_) {
		delete s;
	}
	livings_.clear();
}

void World::simulationStep()
{
	for(Living *s : livings_) {
		(*s).step();
	}
}


std::vector<Ant *> World::getAnts()
{
	std::vector<Ant*> ants;
	for(Living *s : livings_) {
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
	for(Living *s : livings_) {
		Food *a = static_cast<Food*>(s);
		if( a ) {
			foods.push_back(a);
		}
	}
	return foods;
}

std::vector<Living *> World::getClosestLivings(Position mypos, int visibility)
{
	std::vector<Living*> ret;
	for(Living *s : livings_) {
		if ( abs(mypos.posX()-s->posX()) < visibility &&
				abs(mypos.posY()-s->posY()) < visibility ) {
			ret.push_back(s);
		}
	}
}

void World::addLiving(Living*s)
{
	if ( s )
		livings_.push_back(s);
}

void World::removeLiving(Living *l)
{
	/* not implemented yet! */
}
