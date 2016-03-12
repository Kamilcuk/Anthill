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
	/* initialize random seed: */
	srand (time(NULL));

	/* they add itself*/
	/* remember one pheromone map per world! */
	PheromoneMap* to_food = new PheromoneMap(*this, PheromoneMap::Type::ToFood, width, height, 0.01);
	new PheromoneMap(*this, PheromoneMap::Type::FromFood, width, height, 0.01);



	ShapeGenerator shape_gen;

	// draw some pheromones
	for(auto point : shape_gen.GenerateLine(Point(30, 7), Point(30, 25), 4))
		to_food->createBlob(point, 2, 100);
	for(auto point : shape_gen.GenerateCircle(Point(25, 7), 3))
		to_food->createBlob(point, 2, 100);
	for(auto point : shape_gen.GenerateCircle(Point(35, 7), 3))
		to_food->createBlob(point, 2, 100);


	new Ant(*this, Point(30,30));
	new Ant(*this, Point(25,25));
	new Ant(*this, Point(20,20));
	new Ant(*this, Point(50,50));
	new Ant(*this, Point(30,20));

	new Anthill(*this, Point(35,35));

	
	for(auto point : shape_gen.GenerateCircle(Point(15, 15), 3))
	{
		new Food(*this, point);
	}
	for(auto point : shape_gen.GenerateLine(Point(5, 40), Point(20, 20), 2))
	{
		new Food(*this, point);
	}

	for(auto point : shape_gen.GenerateLine(Point(10, 20), Point(30, 30), 3))
	{
		obstacles_.emplace_back(Obstacle(*this, point));
	}
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

/* well, i know */
void World::simulationStep()
{
	std::cout<<"--------------------------------------------"<<std::endl;
	std::vector<Updatable *> updatables2_ = updatables_;
	for(Updatable *s : updatables2_) {
		(*s).step();
	}
	/*
	if ( !(rand()%30) ) {
		int startI = 2+rand()%10;
		int startJ = 2+rand()%10;
		int addI = startI+2+rand()%5;
		int addJ = startJ+2+rand()%5;

		for(int i=startI;i<addI;i++) {
			for(int j=startJ;j<addJ;j++) {
				new Food(*this, Point(i,j));
			}
		}
	}
	*/ // removed randomness for now to test stuff - kosiak
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
std::vector<PheromoneMap*> World::getPheromoneMaps()
{
	/* to pownnien być szablon!!! */
	std::vector<PheromoneMap*> tmp;
	for(Updatable *s : updatables_) {
		PheromoneMap *a = dynamic_cast<PheromoneMap*>(s);
		if( a ) {
			tmp.push_back(a);
		}
	}
	return tmp;
}

std::vector<Obstacle*> World::getObstacles()
{
	std::vector<Obstacle*> ret;
	for(auto o : obstacles_) ret.emplace_back(&o);
	return ret;
}
