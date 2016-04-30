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
        Point pos(rand() % world->width, rand() % (world->height));
            
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
        
    int delta = params.max_dist_from_anthill - params.min_dist_from_anthill;
    
    // todo: more anthills
    auto anthill_pos = world->getSimulationObjects<Anthill>()[0]->getPos();
    
    int num_spawned = 0;
    while(num_spawned < params.quantity)
    {
        Point pos(anthill_pos.posX() + rand() % delta * randSign(),
            anthill_pos.posY() + rand() % delta * randSign());
        
        // check map bounds
        if (!pos.isInBounds(world->width, world->height))
            continue;

        // check if no obstacle there
        bool ok=true;
        for(auto a : world->getSimulationObjects<Obstacle>()){
            if(a->getPos() == pos){
                // collision detected
                ok=false;
                break;
            }
        }
        if(!ok)
            continue;
            
        world->addSimulationObject<Creature>(
            boost::make_shared<Ant>(world, pos));

        // all correct
        num_spawned++;
    }   
}

void WorldGenerator::placeObstacles(World* world, ObstaclesParams& params)
{
    if(params.quantity < 0) 
        throw std::runtime_error("Invalid quantity of obstacles");
        
    int num_spawned = 0;
    while(num_spawned < params.quantity)
    {       
        for(auto point : generateBlob<Obstacle>(params.blob, 
                world->width, world->height))
        {
            if(!point.isInBounds(world->width, world->height))
                continue;
                
            world->addSimulationObject<Obstacle>(
                boost::make_shared<Obstacle>(world, point));
        }
    
        // all correct
        num_spawned++;
    }  
}

void WorldGenerator::placeFoods(World* world, FoodsParams& params)
{
    if(params.quantity < 0) 
        throw std::runtime_error("Invalid quantity of foods");
        
    int num_spawned = 0;
    while(num_spawned < params.quantity)
    {       
        for(auto point : generateBlob<Food>(params.blob, 
                world->width, world->height))
        {
            if(!point.isInBounds(world->width, world->height))
                continue;

            world->addSimulationObject<Food>(
                boost::make_shared<Food>(world, point));
        }
                    
        // all correct
        num_spawned++;
    }  

}
