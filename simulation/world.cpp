/*
 * World.cpp
 *
 *  Created on: 9 mar 2016
 *      Author: ms
 */

#include "world.hpp"

#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <fstream>

#include <boost/make_shared.hpp>

#include "serialization.hpp"
#include "serializationCustom.hpp"

#include "shapeGenerator.hpp"
#include "worldGenerator.hpp"

#include "anthill.hpp"
#include "pheromoneMap.hpp"
#include "ant.hpp"
#include "entity.hpp"
#include "food.hpp"
#include "bodyParts.hpp"
#include "statistics.hpp"

using std::vector;


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
	// init random seed
	srand (time(NULL));

    statistics_ = boost::make_shared<Statistics>(this);

    pheromone_maps_.emplace_back(
        trackEntity<PheromoneMap>(
            boost::make_shared<PheromoneMap>(this, PheromoneMap::Type::ToFood, 
                width, height, 0.1)));
    pheromone_maps_.emplace_back(
        trackEntity<PheromoneMap>(
            boost::make_shared<PheromoneMap>(this, PheromoneMap::Type::FromFood, 
                width, height, 0.1)));
}

void World::stopSimulation()
{
    foods_.clear();
    obstacles_.clear();
    ants_.clear();
    anthills_.clear();
    pheromone_maps_.clear();
    
    entity_ptrs_.clear();
    
	updatable_ptrs_.clear();
    visitable_ptrs_.clear();
}

void World::simulationStep()
{   
    for(auto u : updatable_ptrs_)
        u->step(1);
    for(auto u : updatable_ptrs_)
        u->step(0);
    
    // remove from notification list if flagged to remove
    updatable_ptrs_.erase(
        std::remove_if(updatable_ptrs_.begin(), updatable_ptrs_.end(),
            [] (Updatable* x) -> bool { return x->isFlaggedToRemove(); }), 
        updatable_ptrs_.end());
}

void World::saveState(std::string filename)
{
    std::cout << "Saving simulation state..." << std::endl;
    std::ofstream file(filename);
    boost::archive::text_oarchive out_archive(file);
    
    out_archive << foods_;
    out_archive << obstacles_;
    out_archive << ants_;
    out_archive << anthills_;
    out_archive << pheromone_maps_;
}

void World::loadState(std::string filename)
{
    std::cout << "Loading simulation state..." << std::endl;
    std::ifstream file(filename);
    boost::archive::text_iarchive in_archive(file);
    
    g_world = this; // see serialization.hpp for info on this line
    
    in_archive >> foods_;
    in_archive >> obstacles_;
    in_archive >> ants_;
    in_archive >> anthills_;
    in_archive >> pheromone_maps_;
}

std::vector<boost::weak_ptr<Entity> >& World::getEntityPtrs()
{ 
    if(invalid_entities_)
    {
        // remove expired stuff from entities list
        invalid_entities_ = false;
        entity_ptrs_.erase(
            std::remove_if(entity_ptrs_.begin(), entity_ptrs_.end(),
                [] (boost::weak_ptr<Entity> e) -> bool { return e.expired(); }),
            entity_ptrs_.end());
    }

    return entity_ptrs_; 
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

void World::invalidateEntities()
{
    invalid_entities_ = true;
}
