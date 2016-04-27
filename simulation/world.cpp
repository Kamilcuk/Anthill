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

	statistics_ = boost::make_shared<Statistics>();

    pheromone_maps_.emplace_back(
        boost::make_shared<PheromoneMap>(this, PheromoneMap::Type::ToFood, 
            width, height, 0.1));
    pheromone_maps_.emplace_back(
        boost::make_shared<PheromoneMap>(this, PheromoneMap::Type::FromFood, 
            width, height, 0.1));
}

void World::stopSimulation()
{   
    foods_.clear();
    obstacles_.clear();
    creatures_.clear();
    anthills_.clear();
    pheromone_maps_.clear();
    
    getEntityPtrs();    // just to clear expired weak_ptrs
    entity_ptrs_.clear();
    
	updatable_ptrs_.clear();
    visitable_ptrs_.clear();
}

void World::simulationStep()
{   
    for(auto u : updatable_ptrs_)
		u->step(1);

	// we can track visitables that want to be erased/die
	statistics_->update(this->getVisitablePtrs());

    for(auto u : updatable_ptrs_)
        u->step(0);
    
    // remove from simulation if flagged to remove.
    // We're using "raw" entity_ptrs_ instead of getEntityPtrs() because
    // getEntityPtrs() removes expired pointers from the vector and that would
    // cause size of the vector to change mid-iteration -> undefined behaviour.
    // std::for_each(entity_ptrs_.begin(), entity_ptrs_.end(),
    //         [] (boost::weak_ptr<Entity> e) { e.lock()->removeIfFlagged(); });
}

void World::saveState(std::string filename)
{
    std::cout << "Saving simulation state to " << filename << std::endl;
    std::ofstream file(filename);
    boost::archive::text_oarchive out_archive(file);
    
    out_archive << *this;
    out_archive << foods_;
    out_archive << obstacles_;
    out_archive << creatures_;
    out_archive << anthills_;
    out_archive << pheromone_maps_;
    
    std::cout << "Finished saving" << std::endl;
}

void World::loadState(std::string filename)
{
    std::cout << "Loading simulation state from " << filename << std::endl;
    std::ifstream file(filename);
    boost::archive::text_iarchive in_archive(file);
    
    g_world = this; // see serialization.hpp for info on this line
    
    in_archive >> *this;
    
    in_archive >> foods_;
    in_archive >> obstacles_;
    in_archive >> creatures_;
    in_archive >> anthills_;
    in_archive >> pheromone_maps_;
    
    for(const auto& entity : foods_) trackEntity(entity);
    for(const auto& entity : obstacles_) trackEntity(entity);
    for(const auto& entity : creatures_) trackEntity(entity);
    for(const auto& entity : anthills_) trackEntity(entity);
    
    std::cout << "Finished loading" << std::endl;
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

void World::trackEntity(boost::shared_ptr<Entity> e)
{
    entity_ptrs_.emplace_back(boost::weak_ptr<Entity>(e));
}

void World::invalidateEntities()
{
    invalid_entities_ = true;
}


// getters

template<>
const std::vector<boost::shared_ptr<Food> >& 
    World::getSimulationObjects<Food>() const
{ return foods_; }

template<>
const std::vector<boost::shared_ptr<Obstacle> >& 
    World::getSimulationObjects<Obstacle>() const
{ return obstacles_; }

template<>
const std::vector<boost::shared_ptr<Creature> >& 
    World::getSimulationObjects<Creature>() const
{ return creatures_; }

template<>
const std::vector<boost::shared_ptr<Anthill> >& 
    World::getSimulationObjects<Anthill>() const
{ return anthills_; }

template<>
const std::vector<boost::shared_ptr<PheromoneMap> >& 
    World::getSimulationObjects<PheromoneMap>() const
{ return pheromone_maps_; }


// adders

template<>
boost::shared_ptr<Food> 
    World::addSimulationObject<Food>(boost::shared_ptr<Food> obj) 
{ 
    trackEntity(obj);
    foods_.emplace_back(obj);
    return obj; 
}

template<>
boost::shared_ptr<Obstacle> 
    World::addSimulationObject<Obstacle>(boost::shared_ptr<Obstacle> obj) 
{ 
    trackEntity(obj);
    obstacles_.emplace_back(obj);
    return obj; 
}

template<>
boost::shared_ptr<Creature> 
    World::addSimulationObject<Creature>(boost::shared_ptr<Creature> obj) 
{ 
    trackEntity(obj);
    creatures_.emplace_back(obj);
    return obj; 
}

template<>
boost::shared_ptr<Anthill> 
    World::addSimulationObject<Anthill>(boost::shared_ptr<Anthill> obj) 
{ 
    trackEntity(obj);
    anthills_.emplace_back(obj);
    return obj; 
}

template<>
boost::shared_ptr<PheromoneMap> 
    World::addSimulationObject<PheromoneMap>(boost::shared_ptr<PheromoneMap> obj) 
{ 
    pheromone_maps_.emplace_back(obj);
    return obj; 
}
