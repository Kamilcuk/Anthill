#ifndef WORLDGENPARAMS_H
#define WORLDGENPARAMS_H

#include <set>

#include "point.hpp"

/// Parameters of a random blob of points. This struct is used by other structs
/// that require some kind of random blob creation.
/// All default field values can be created as base values, which are multiplied
/// by some coefficient as needed.
struct RandomBlob
{
    // Base field values:
    float chance_line = 0.6; 
    
    int line_size_min = 5;
    int line_size_max = 50;
    int line_width_min = 3;
    int line_width_max = 12;
    
    int circle_radius_min = 2;
    int circle_radius_max = 5;
    
    // Randomly generates a set of points in a shape of a line or a circle,
    // depending on field values. Also, clips resulting set of points
    // to provided size_x, size_y params.
    std::set<Point> generate(int size_x, int size_y);
};

/// Struct representing params for creating anthills. 
struct AnthillParams
{
    int quantity = 1;
};

/// Struct representing params for spawning ants. 
/// All default field values can be created as base values, which are multiplied
/// by some coefficient as needed.
struct AntsParams
{
    int quantity = 1;
    
    // defines a ring in which ants can be placed
    int min_dist_from_anthill = 5;
    int max_dist_from_anthill = 10;
    
    // modify default params based on a singular value - desired number of ants
    void applyNumAnts(unsigned desired_num_ants);
};

/// Struct representing params for spawning obstacles.
/// All default field values can be created as base values, which are multiplied
/// by some coefficient as needed.
struct ObstaclesParams
{
    RandomBlob blob;
    int quantity_per_100_by_100 = 1000;
    
    // scale default params based on "obstacle_freq" param
    void applyObstacleFrequency(unsigned obstacle_freq);
};

/// Struct representing params for spawning foods.
/// All default field values can be created as base values, which are multiplied
/// by some coefficient as needed.
struct FoodsParams
{
    RandomBlob blob;
    int quantity_per_100_by_100 = 700;
    FoodsParams() 
    { 
        blob.line_size_min = 1; 
        blob.line_size_max = 15; 
        blob.line_width_max = 10;
        blob.circle_radius_min = 1;
        blob.circle_radius_max = 4;
    }
    
     // scale default params based on "food_generosity" param
    void applyFoodGenerosity(unsigned food_generosity);
}; 

#endif // WORLDGENPARAMS_H