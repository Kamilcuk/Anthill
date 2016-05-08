#ifndef WORLD_GENERATOR_H
#define WORLD_GENERATOR_H

#include <stdlib.h>
#include <math.h>
#include <set>

#include "point.hpp"
#include "shapeGenerator.hpp"

class World;
class Anthill;
class Ant;
class Obstacle;
class Food;

struct AnthillParams;
struct AntsParams;
struct ObstaclesParams;
struct FoodsParams;


/// Parameters of a random blob of points.
struct BlobParams
{
    // need to define this struct here because were using it in a template method
    float chance_line = 0.6; 
    int line_size_min = 5;
    int line_size_max = 50;
    int line_width_min = 3;
    int line_width_max = 12;
    
    int circle_radius_min = 2;
    int circle_radius_max = 5;
};


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
    
    // Randomly generates a set of points in a shape of a line or a circle,
    // depending on supplied params. Also, clips resulting set of points
    // to provided size_x, size_y params.
    template<class C>
    static std::set<Point> generateBlob(BlobParams params, int size_x, 
        int size_y)
    {
        if(params.line_size_min < 0 || params.line_size_max < 0 || 
                params.line_size_min > params.line_size_max || 
                params.circle_radius_min < 0 || params.circle_radius_max < 0 || 
                params.circle_radius_min > params.circle_radius_max || 
                params.chance_line < 0 || params.chance_line > 1)
            throw std::runtime_error("Invalid blob params.");
            
        Point seed(rand() % size_x, rand() % size_y);
        
        if(rand() % 100 < int(params.chance_line * 100.0))
        {
            // generate line
            int len = rand() % (params.line_size_max - params.line_size_min);
            float angle = rand() % 360;
            float angle_rad = angle * 2 * 3.1415 / 360.0;
            Point dest(seed.posX() + len * cos(angle_rad), 
                seed.posY() + len * sin(angle_rad));
            int thick = rand() % 
                (params.line_width_max - params.line_width_min);
            return ShapeGenerator::GenerateLine(seed, dest, thick);
        }
        else 
        {
            // generate circle
            int radius = rand() % 
                (params.circle_radius_max - params.circle_radius_min);
            return ShapeGenerator::GenerateCircle(seed, radius);
        }
    }
};

struct AnthillParams
{
    int quantity = 1;
};

struct AntsParams
{
    int quantity = 2;
    int min_dist_from_anthill = 5;
    int max_dist_from_anthill = 15;
};

struct ObstaclesParams
{
    BlobParams blob;
    int quantity = 20;
};

struct FoodsParams
{
    BlobParams blob;
    int quantity = 30;
    FoodsParams() 
    { 
        blob.line_size_min = 1; 
        blob.line_size_max = 15; 
        blob.line_width_max = 10;
        blob.circle_radius_min = 1;
        blob.circle_radius_max = 4;
    }
}; 

#endif // WORLD_GENERATOR_H
