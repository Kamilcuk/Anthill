#include "worldGenParams.hpp"

#include <math.h>
#include <iostream>
#include "shapeGenerator.hpp"

std::set<Point> RandomBlob::generate(int size_x, int size_y)
{
    if(line_size_min < 0 || line_size_max < 0 || 
            line_size_min > line_size_max || 
            circle_radius_min < 0 || circle_radius_max < 0 || 
            circle_radius_min > circle_radius_max || 
            chance_line < 0 || chance_line > 1)
        throw std::runtime_error("Invalid blob ");
        
    Point seed(rand() % size_x, rand() % size_y);
    
    if(rand() % 100 < int(chance_line * 100.0))
    {
        // generate line
        int len = rand() % (line_size_max - line_size_min);
        float angle = rand() % 360;
        float angle_rad = angle * 2 * 3.1415 / 360.0;
        Point dest(seed.posX() + len * cos(angle_rad), 
            seed.posY() + len * sin(angle_rad));
        int thick = rand() % (line_width_max - line_width_min);
        return ShapeGenerator::GenerateLine(seed, dest, thick);
    }
    else 
    {
        // generate circle
        int radius = rand() % (circle_radius_max - circle_radius_min);
        return ShapeGenerator::GenerateCircle(seed, radius);
    }
}

void AntsParams::applyNumAnts(unsigned desired_num_ants)
{
    quantity = desired_num_ants;
    
    // calc upper limit for radius of spawning ants based on area of the ring
    const float smaller_circle_area = 3.14159 * min_dist_from_anthill * 
        min_dist_from_anthill;
    float larger_circle_area = 3.14159 * max_dist_from_anthill * 
        max_dist_from_anthill;
        
    // coef to make it so that randomly spawned ants aren't literally all
    // right next to each other - make spawning area bigger by this coef
    const float empty_space_coef = 2.0; 
    
    while(floor(larger_circle_area - smaller_circle_area) < 
        desired_num_ants * empty_space_coef)
    {
        max_dist_from_anthill += 1;
        larger_circle_area = 3.14159 * max_dist_from_anthill * 
            max_dist_from_anthill;
    }    
}

void ObstaclesParams::applyObstacleFrequency(unsigned obstacle_freq)
{
    // treat default params as params at obstacle_freq = 50
    quantity_per_100_by_100 *= (obstacle_freq * 2.0) / 100.0;
}

void FoodsParams::applyFoodGenerosity(unsigned food_generosity)
{
    // treat default params as params at food_generosity = 50
    quantity_per_100_by_100 *= (food_generosity * 2.0) / 100.0;
}