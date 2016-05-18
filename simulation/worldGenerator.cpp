#include "worldGenerator.hpp"

#include <stdexcept>
#include <algorithm>
#include <set>

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
    for(unsigned i = 0; i < params.quantity; i++)
    {
        Point pos(rand() % world->width_, rand() % (world->height_));
            
        world->addSimulationObject<Anthill>(
            boost::make_shared<Anthill>(world, pos));
    }
}

void WorldGenerator::placeAnts(World* world, AntsParams& params)
{
    if(params.min_dist_from_anthill > params.max_dist_from_anthill) 
        throw std::runtime_error("Min dist can't be higher than max dist");
    if(world->getSimulationObjects<Anthill>().size() == 0) 
        throw std::runtime_error("No anthill!");
        
    auto free_spaces = getFreeSpaces(world);
    unsigned num_spawned = 0;
        
    while(num_spawned < params.quantity)
    {        
        if(free_spaces.empty())
            throw std::runtime_error(
                "WorldGenerator::placeAnts no available empty space");

        Point pos = fitNewPos(world, params, free_spaces); 
        
        world->addSimulationObject<Creature>(
            boost::make_shared<Ant>(world, pos, 
                decideAntType(num_spawned, params)));
        num_spawned++;
    }   
}

void WorldGenerator::placeObstacles(World* world, ObstaclesParams& params)
{
    if(params.quantity_per_100_by_100 > 10000) 
        throw std::runtime_error("Invalid quantity per 100 by 100 of obstacle");
        
    const int num_to_spawn = params.quantity_per_100_by_100 * 
        world->width_ * world->height_ / 10000;

    int num_spawned = 0;
    while(num_spawned < num_to_spawn)
    {       
        for(const auto& point : params.blob.generate( 
                world->width_, world->height_))
        {
            if(num_spawned > num_to_spawn) break;
            
            if(!point.isInBounds(world->width_, world->height_))
                continue;
                
            world->addSimulationObject<Obstacle>(
                boost::make_shared<Obstacle>(world, point));
                
            num_spawned++;
        }
    }  
}

void WorldGenerator::placeFoods(World* world, FoodsParams& params)
{
    if(params.quantity_per_100_by_100 > 10000) 
        throw std::runtime_error("Invalid quantity per 100 by 100 of foods");
        
    const unsigned num_to_spawn = params.quantity_per_100_by_100 * 
        world->width_ * world->height_ / 10000;
        
    unsigned num_spawned = 0;
    while(num_spawned < num_to_spawn)
    {       
        for(const auto& point : params.blob.generate( 
                world->width_, world->height_))
        {
            if(num_spawned > num_to_spawn) break;
            
            if(!point.isInBounds(world->width_, world->height_))
                continue;

            world->addSimulationObject<Food>(
                boost::make_shared<Food>(world, point));

            num_spawned++;
        }       
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

std::set<Point> WorldGenerator::getTakenSpaces(World* world)
{
    // using set because entities may overlap but we want unique spaces
    auto obstacles = world->getSimulationObjects<Obstacle>();
    std::set<Point> s;
    for(const auto& o : obstacles) s.insert(o->getPos());
    return s;
}

std::vector<Point> WorldGenerator::getFreeSpaces(World* world)
{
    std::vector<Point> v = getAllPoints(world);
    for(const auto& t : getTakenSpaces(world))
        v.erase(std::remove(v.begin(), v.end(), t), v.end());
    return v;
}

std::vector<Point> WorldGenerator::getAllPoints(World* world)
{
    std::vector<Point> v;
    auto dim = world->getDimensions();
    for(unsigned x = 0; x < (unsigned)dim.posX(); x++)
        for(unsigned y = 0; y < (unsigned)dim.posY(); y++)
            v.emplace_back(Point(x, y));
    return v;
}

Point WorldGenerator::fitNewPos(World* world, AntsParams& params,
    std::vector<Point>& free_spaces)
{
    // todo: handle more anthills
    const auto anthill_pos = 
        world->getSimulationObjects<Anthill>()[0]->getPos();
        
    Point pos;
    unsigned failed_counter = 0; 
    const unsigned failed_threshold = 100; 
    do
    {
        pos = free_spaces[rand() % free_spaces.size()];
        failed_counter++;
        
        if(failed_counter > failed_threshold)
        {
            failed_counter = 0;
            params.max_dist_from_anthill *= 2;
        }    
    } while( 
        pos.getDistance(anthill_pos) < params.min_dist_from_anthill ||
        pos.getDistance(anthill_pos) > params.max_dist_from_anthill);
        
    free_spaces.erase(
            std::remove(free_spaces.begin(), free_spaces.end(), pos), 
        free_spaces.end());
        
    return pos;
}

Ant::Type WorldGenerator::decideAntType(const int num_spawned, AntsParams& params)
{
    if(num_spawned == 0)
        // spawn one queen
        return Ant::Type::Queen;
    else if(num_spawned <= params.ratio_scouts * params.quantity)
        return Ant::Type::Scout;
    else
        return Ant::Type::Worker;
}