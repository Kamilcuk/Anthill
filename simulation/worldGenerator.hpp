#ifndef WORLD_GENERATOR_H
#define WORLD_GENERATOR_H

#include <stdlib.h>
#include <math.h>
#include <set>
#include <vector>

#include "point.hpp"
#include "worldGenParams.hpp"
#include "ant.hpp"

class World;
class Anthill;
class Obstacle;
class Food;

/**
 * Utility class responsible for randomly generating simulation world.
 */
class WorldGenerator
{
public:

    // structs holding default params for generating specific types of objects.
    static AnthillParams default_anthill_params;
    static AntsParams default_ants_params;
    static ObstaclesParams default_obstacles_params;
    static FoodsParams default_food_params;

    // Following methods will modify world state by adding new objects to 
    // simulation state.
    static void placeAnthill(World* world, 
        AnthillParams& params = WorldGenerator::default_anthill_params);
    static void placeAnts(World* world, 
        AntsParams& params = WorldGenerator::default_ants_params);
    static void placeObstacles(World* world, 
        ObstaclesParams& params = WorldGenerator::default_obstacles_params);
    static void placeFoods(World* world, 
        FoodsParams& params = WorldGenerator::default_food_params);
        
    /// Initializes pheromone maps with specified pheromone decay rates.
    /// Number and types of pheromone maps is hardcoded.
    static void initPheromoneMaps(World* world, float to_food_decay_rate,
        float from_food_decay_rate, float anthill_decay_rate);
        
private:
    inline static int randSign()
    {
        return (rand() % 2 ? 1 : -1);
    }
    
    static std::set<Point> getTakenSpaces(World* world);
    static std::vector<Point> getFreeSpaces(World* world);
    static std::vector<Point> getAllPoints(World* world);
    static Point fitNewPos(World* world, AntsParams& params, 
        std::vector<Point>& free_spaces);
    static Ant::Type decideAntType(const int num_spawned, AntsParams& params);
};


#endif // WORLD_GENERATOR_H
