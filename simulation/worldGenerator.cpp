#include "worldGenerator.hpp"

#include <stdexcept>

#include <boost/make_shared.hpp>

#include "world.hpp"
#include "point.hpp"

#include "anthill.hpp"
#include "creature.hpp"
#include "ant.hpp"
#include "obstacle.hpp"
#include "food.hpp"

// initialize static members
#define INIT_STATIC(cl, st, val) st cl::val = st();
INIT_STATIC(WorldGenerator, AnthillParams, default_anthill_params);
INIT_STATIC(WorldGenerator, AntsParams, default_ants_params);
INIT_STATIC(WorldGenerator, ObstaclesParams, default_obstacles_params);
INIT_STATIC(WorldGenerator, FoodsParams, default_food_params);

void WorldGenerator::placeAnthill(World* world, AnthillParams& params)
{
    for(int i = 0; i < params.quantity; i++)
    {
        Point pos(rand() % world->width_, rand() % (world->height_));
            
        world->addSimulationObject<Anthill>(
            boost::make_shared<Anthill>(world, pos));
    }
}

void WorldGenerator::placeAnts(World* world, AntsParams& params)
{
    if(params.quantity < 0) 
        throw std::runtime_error("Invalid quantity of ants");
    if(params.min_dist_from_anthill > params.max_dist_from_anthill) 
        throw std::runtime_error("Min dist can't be higher than max dist");
    if(world->getSimulationObjects<Anthill>().size() == 0) 
        throw std::runtime_error("No anthill!");
        
    const int delta = params.max_dist_from_anthill - 
        params.min_dist_from_anthill;
    
    // todo: handle more anthills
    auto anthill_pos = world->getSimulationObjects<Anthill>()[0]->getPos();
    
    int num_spawned = 0;
    while(num_spawned < params.quantity)
    {
        Point pos(anthill_pos.posX() + rand() % delta * randSign(),
            anthill_pos.posY() + rand() % delta * randSign());
        
        if (!pos.isInBounds(world->width_, world->height_))
            continue;

        bool collision_detected = false;
        for(const auto& obstacle : world->getSimulationObjects<Obstacle>())
        {
            if(obstacle->getPos() == pos)
            {
                collision_detected = true;
                break;
            }
        }
        if(collision_detected)
            continue;
            
        if(num_spawned == 0)
        {
            // spawn one queen
            world->addSimulationObject<Creature>(
                boost::make_shared<Ant>(world, pos, Ant::Type::Queen));
        }
        else
        {
            world->addSimulationObject<Creature>(
                boost::make_shared<Ant>(world, pos, Ant::Type::Worker));
        }

        num_spawned++;
    }   
}

void WorldGenerator::placeObstacles(World* world, ObstaclesParams& params)
{
    if(params.quantity_per_100_by_100 < 0) 
        throw std::runtime_error("Invalid quantity per 100 by 100 of obstacles");
        
    const int num_to_spawn = params.quantity_per_100_by_100 * 
        world->width_ * world->height_ / 10000.0;
        
    int num_spawned = 0;
    while(num_spawned < num_to_spawn)
    {       
        for(const auto& point : params.blob.generate( 
                world->width_, world->height_))
        {
            if(!point.isInBounds(world->width_, world->height_))
                continue;
                
            world->addSimulationObject<Obstacle>(
                boost::make_shared<Obstacle>(world, point));
        }
    
        num_spawned++;
    }  
}

void WorldGenerator::placeFoods(World* world, FoodsParams& params)
{
    if(params.quantity_per_100_by_100 < 0) 
        throw std::runtime_error("Invalid quantity per 100 by 100 of foods");
        
    const int num_to_spawn = params.quantity_per_100_by_100 * 
        world->width_ * world->height_ / 10000.0;
        
    int num_spawned = 0;
    while(num_spawned < num_to_spawn)
    {       
        for(const auto& point : params.blob.generate( 
                world->width_, world->height_))
        {
            if(!point.isInBounds(world->width_, world->height_))
                continue;

            world->addSimulationObject<Food>(
                boost::make_shared<Food>(world, point));
        }
                    
        num_spawned++;
    }  
}

void WorldGenerator::initPheromoneMaps(World* world, float to_food_decay_rate,
    float from_food_decay_rate, float anthill_decay_rate)
{
    world->pheromone_maps_.emplace_back(
        boost::make_shared<PheromoneMap>(world, PheromoneMap::Type::ToFood, 
            world->width_, world->height_, to_food_decay_rate));
    world->pheromone_maps_.emplace_back(
        boost::make_shared<PheromoneMap>(world, PheromoneMap::Type::FromFood, 
            world->width_, world->height_, from_food_decay_rate));
    world->pheromone_maps_.emplace_back(
        boost::make_shared<PheromoneMap>(world, PheromoneMap::Type::Anthill, 
            world->width_, world->height_, anthill_decay_rate));
}