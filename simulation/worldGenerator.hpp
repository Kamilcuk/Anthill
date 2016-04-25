#ifndef WORLD_GENERATOR_H
#define WORLD_GENERATOR_H

#include <stdlib.h>

class World;
class Point;
class Anthill;
class Ant;
class Obstacle;
class Food;

/**
 * Responsible for randomly generating simulation world.
 */
class WorldGenerator
{
public:

    struct AnthillParams
    {
        int quantity = 1;
    };
    
    struct AntsParams
    {
        int quantity = 5;
        int min_dist_from_anthill = 1;
        int max_dist_from_anthill = 5;
    };
    
    struct ObstaclesParams
    {
        int size_variance = 15;
        int size_min = 5;
        int size_max = 20;
    };
    
    struct FoodParams
    {
        int size_variance = 5;
        int size_min = 1;
        int size_max = 10;
    }; 
    
    static AnthillParams default_anthill_params;
    static AntsParams default_ants_params;
    static ObstaclesParams default_obstacles_params;
    static FoodParams default_food_params;

    static void placeAnthill(World* world, 
        AnthillParams& params = WorldGenerator::default_anthill_params);
    static void placeAnts(World* world, 
        AntsParams& params = WorldGenerator::default_ants_params);
    static void placeObstacles(World* world, 
        ObstaclesParams& params = WorldGenerator::default_obstacles_params);
    static void placeFood(World* world, 
        FoodParams& params = WorldGenerator::default_food_params);
        
private:
    inline static int randSign()
    {
        return (rand() % 2 ? 1 : -1);
    }
};

#endif // WORLD_GENERATOR_H