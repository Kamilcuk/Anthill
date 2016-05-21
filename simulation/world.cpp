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
#include <thread>
#include <fstream>

#include <boost/make_shared.hpp>

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


World::World()
{    
    multithreaded_ = false;
    setDimensions(300, 200);

    /* initialize random seed: */
	srand (time(NULL));
}

World::~World()
{
    this->stopSimulation();
}

void World::setDimensions(unsigned x, unsigned y)
{
	width_ = x;
	height_ = y;
    entity_map_ = boost::make_shared<Entity2DMap>(x, y);
}

Point World::getDimensions()
{
    return Point(width_, height_);
}

void World::setMultithreaded(bool m)
{
    multithreaded_ = m;
}

bool World::isMultithreaded()
{
    return multithreaded_;
}

void World::startSimulation()
{
	srand (time(NULL));

	statistics_ = boost::make_shared<Statistics>();
}

void World::stopSimulation()
{   
    foods_.clear();
    obstacles_.clear();
    creatures_.clear();
    anthills_.clear();
    pheromone_maps_.clear();
    
	updatable_ptrs_.clear();
    visitable_ptrs_.clear();
}

void World::simulationStep()
{      
    updateAll(1);
    entity_map_->applyAllMoves();
    
	// we can track visitables that want to be erased/die
	if(statistics_)
		statistics_->update(*this, statisticsEnabled_);
        
    updateAll(0);
    entity_map_->applyAllMoves();
    
    // remove simulation objects that were flagged to remove;
    removeFlaggedSimulationObjects<Food>();
    removeFlaggedSimulationObjects<Obstacle>();
    removeFlaggedSimulationObjects<Creature>();
    removeFlaggedSimulationObjects<Anthill>();
    
    entity_map_->removeExpired();
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
    if(e)
        entity_map_->add(boost::weak_ptr<Entity>(e));
}

void World::updateAll(int stepsize)
{
    if(isMultithreaded())
        updateAllInParallel(stepsize);
    else
        updateAllInSerial(stepsize);
}

void World::updateAllInSerial(int stepsize)
{
    for(auto u : updatable_ptrs_)
        u->step(stepsize);
}

void World::updateAllInParallel(int stepsize)
{
    auto worker_func = 
        [] (auto begin, auto end, int stepsize)
        {
            for(auto updatable_iter = begin; updatable_iter != end; 
                ++updatable_iter)
            {
                (*updatable_iter)->step(stepsize);
            }
        }
    ;
    
    static const unsigned num_workers = std::thread::hardware_concurrency();
    unsigned per_worker = updatable_ptrs_.size() / num_workers;
    auto chunk_start = updatable_ptrs_.begin();
    std::vector<std::thread> workers(num_workers);
    for(auto w = workers.begin(); w != workers.end() - 1; ++w)
    {
        *w = std::thread(worker_func, chunk_start, chunk_start + per_worker, 
            stepsize);
        chunk_start += per_worker;
    }
    // set last worker's end iterator to .end() in case per_worker is not even
    workers.back() = std::thread(worker_func, chunk_start, 
        updatable_ptrs_.end(), stepsize); 

    for(auto&& w : workers)
        w.join();
}


// Following are getSimulationObjects() template specializations. 
// We need those, because we want to map a generic template method into
// pre-defined set of storage vectors.

template<>
World::VectorOfSharedPtrs<Food>& 
    World::getSimulationObjects<Food>()
{ return foods_; }

template<>
World::VectorOfSharedPtrs<Obstacle>& 
    World::getSimulationObjects<Obstacle>()
{ return obstacles_; }

template<>
World::VectorOfSharedPtrs<Creature>& 
    World::getSimulationObjects<Creature>()
{ return creatures_; }

template<>
World::VectorOfSharedPtrs<Anthill>& 
    World::getSimulationObjects<Anthill>()
{ return anthills_; }

template<>
World::VectorOfSharedPtrs<PheromoneMap>& 
    World::getSimulationObjects<PheromoneMap>()
{ return pheromone_maps_; }
