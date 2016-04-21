/*
 * World.cpp
 *
 *  Created on: 9 mar 2016
 *      Author: ms
 */

#include "world.hpp"
#include "anthill.hpp"
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <cstdlib>
#include <ctime>
#include <boost/make_shared.hpp>
#include "pheromoneMap.hpp"
#include "ant.hpp"
#include "entity.hpp"
#include "food.hpp"
#include <algorithm>
#include "bodyParts.hpp"
#include "statistics.hpp"

using std::vector;


World::World() : statistics_(this)
{    
    filename_ = "simulation_state";
    
    setDimensions(300, 200);

    /* initialize random seed: */
	srand (time(NULL));
}

World::~World()
{
    this->stopSimulation();
}

void World::setDimensions(int X, int Y)
{
	width = X;
	height = Y;
}
void World::setSimulationFramerate(float)
{
}
void World::startSimulation()
{
	/* initialize random seed: */
	srand (time(NULL));

	/* they add itself*/
    /* remember one pheromone map per world! */
    pheromone_maps_.emplace_back(PheromoneMap(this, 
        PheromoneMap::Type::ToFood, width, height, 0.1));
    pheromone_maps_.emplace_back(PheromoneMap(this, 
        PheromoneMap::Type::FromFood, width, height, 0.1));
                
    ants_.emplace_back(Ant(this, Point(30,30)));
    ants_.emplace_back(Ant(this, Point(25,25)));
    ants_.emplace_back(Ant(this, Point(20,20)));
    ants_.emplace_back(Ant(this, Point(50,50)));
    ants_.emplace_back(Ant(this, Point(30,20)));

    anthills_.emplace_back(Anthill(this, Point(35,35)));

	ShapeGenerator shape_gen;
	for(auto point : shape_gen.GenerateCircle(Point(15, 30), 3))
	    foods_.emplace_back(Food(this, point));
	for(auto point : shape_gen.GenerateLine(Point(5, 40), Point(20, 20), 2))
	    foods_.emplace_back(Food(this, point));
}

void World::stopSimulation()
{
    foods_.clear();
    obstacles_.clear();
    ants_.clear();
    anthills_.clear();
    pheromone_maps_.clear();
    
	updatable_ptrs_.clear();
    entity_ptrs_.clear();
    visitable_ptrs_.clear();
}

void World::simulationStep()
{
    std::cout << "updating number:" << updatable_ptrs_.size() << std::endl;
    
    for(auto u : updatable_ptrs_)
        u->step(1);
    for(auto u : updatable_ptrs_)
        u->step(0);
    
    // remove from notification list if flagged to remove
    updatable_ptrs_.erase(
        std::remove_if(updatable_ptrs_.begin(), updatable_ptrs_.end(),
            [] (Updatable* x) -> bool { return x->isFlaggedToRemove(); }), 
        updatable_ptrs_.end());
    
    statistics_.step(1);
}

void World::serializeState()
{
    std::cout << "serializeState TODO\n";
}

void World::deserializeState()
{
    std::cout << "deserializeState TODO\n";
}

void World::addUpdatable(Updatable* u)
{
    updatable_ptrs_.emplace_back(u);
}

void World::removeUpdatable(Updatable* u)
{  
    updatable_ptrs_.erase(
        std::remove(updatable_ptrs_.begin(), updatable_ptrs_.end(), u), 
        updatable_ptrs_.end());
}

void World::addEntity(Entity* e)
{
    entity_ptrs_.emplace_back(e);
}

void World::removeEntity(Entity* e)
{
    entity_ptrs_.erase(
        std::remove(entity_ptrs_.begin(), entity_ptrs_.end(), e), 
        entity_ptrs_.end());
}

void World::addVisitable(Visitable* v)
{
    visitable_ptrs_.emplace_back(v);
}

void World::removeVisitable(Visitable* v)
{
    visitable_ptrs_.erase(
        std::remove(visitable_ptrs_.begin(), visitable_ptrs_.end(), v), 
        visitable_ptrs_.end());
}    