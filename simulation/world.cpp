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
	Ant *a1 = new Ant(*this, Point(1,1));
	updatables_.push_back(a1);

	//Ant *a2 = new Ant(*this, Position(10,10));
	//simulables_.push_back(a2);

	Food *f1 = new Food(Point(3,3));
	updatables_.push_back(f1);
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
	for(Updatable *s : updatables_) {
		(*s).step();
	}
}


std::vector<Ant *> World::getAnts()
{
	std::vector<Ant*> ants;
	for(Updatable *s : updatables_) {
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
	for(Updatable *s : updatables_) {
		Food *a = static_cast<Food*>(s);
		if( a ) {
			foods.push_back(a);
		}
	}
	return foods;
}

std::vector<Updatable *> World::getClosestLivings(Point mypos, int visibility)
{
	std::vector<Updatable*> ret;
	for(Updatable *s : updatables_) {
		/*if ( abs(mypos.posX()-s->getPos().posX()) < visibility &&
				abs(mypos.posY()-s->getPos().posY()) < visibility ) {
			ret.push_back(s);
		}*/
	}
}

void World::addLiving(Updatable*s)
{
	if ( s )
		updatables_.push_back(s);
}

void World::removeLiving(Updatable *l)
{
	/* not implemented yet! */
}
