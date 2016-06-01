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
    
    unsigned line_size_min = 5;
    unsigned line_size_max = 50;
    unsigned line_width_min = 3;
    unsigned line_width_max = 12;
    
    unsigned circle_radius_min = 2;
    unsigned circle_radius_max = 5;
    
    // Randomly generates a set of points in a shape of a line or a circle,
    // depending on field values. Also, clips resulting set of points
    // to provided size_x, size_y params.
    std::set<Point> generate(unsigned size_x, unsigned size_y);
};

/// Struct representing params for creating anthills. 
struct AnthillParams
{
    unsigned quantity = 1;
};

/// Struct representing params for creating anthills. 
struct PheromoneParams
{
    float to_food_decay_rate = 0.2;
    float from_food_decay_rate = 0.2;
    float anthill_decay_rate = 0.2;
    float scale = 0.07;
    
    /// translates coefficients from 1-100 and applies scale
    void applyCoefficients(unsigned to_food, unsigned from_food, 
        unsigned anthill);
};

/// Struct representing params for spawning ants. 
/// All default field values can be created as base values, which are multiplied
/// by some coefficient as needed.
struct AntsParams
{
    unsigned quantity = 1;
    float ratio_scouts = 0.1;
    
    // Initial dimensions of a ring where ants will be placed. Dimensions
    // are modified by world generator if needed. You can control ant 
    // distribution using these params. For example, if you want uniform 
    // ant distribution, use high value for max_dist_from_anthill.
    // If you want gaussian-like distribution, use low max_dist_from_anthill, 
    // as the world generator will expand it only if it's too hard to find a 
    // spot to spawn a new ant.
    unsigned min_dist_from_anthill = 5;
    unsigned max_dist_from_anthill = 10;
    
    void applyNumAnts(unsigned desired_num_ants);
    void applyRatioScouts(unsigned percent_scouts);
};

/// Struct representing params for spawning obstacles.
/// All default field values can be created as base values, which are multiplied
/// by some coefficient as needed.
struct ObstaclesParams
{
    RandomBlob blob;
    unsigned quantity_per_100_by_100 = 1000;
    
    // scale default params based on "obstacle_freq" (values 0-100) param
    void applyObstacleFrequency(unsigned obstacle_freq);
};

/// Struct representing params for spawning foods.
/// All default field values can be created as base values, which are multiplied
/// by some coefficient as needed.
struct FoodsParams
{
    RandomBlob blob;
    unsigned quantity_per_100_by_100 = 700;
    FoodsParams() 
    { 
        blob.line_size_min = 1; 
        blob.line_size_max = 15; 
        blob.line_width_max = 10;
        blob.circle_radius_min = 1;
        blob.circle_radius_max = 4;
    }
    
     // scale default params based on "food_generosity" (values 0-100) param
    void applyFoodGenerosity(unsigned food_generosity);
}; 

#endif // WORLDGENPARAMS_H
