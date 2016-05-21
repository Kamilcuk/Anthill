#include "worldGenParams.hpp"

#include <math.h>
#include <iostream>
#include "shapeGenerator.hpp"

std::set<Point> RandomBlob::generate(unsigned size_x, unsigned size_y)
{
    if(line_size_min > line_size_max || 
        circle_radius_min > circle_radius_max || 
        chance_line < 0 || chance_line > 1)
        throw std::runtime_error("Invalid blob ");
        
    const Point seed(rand() % size_x, rand() % size_y);
    
    if(rand() % 100 < int(chance_line * 100.0))
    {
        // generate line
        const unsigned len = rand() % (line_size_max - line_size_min);
        const float angle = rand() % 360;
        const float angle_rad = angle * 2 * 3.1415 / 360.0;
        const Point dest(seed.posX() + len * cos(angle_rad), 
            seed.posY() + len * sin(angle_rad));
        const unsigned thick = rand() % (line_width_max - line_width_min);
        return ShapeGenerator::GenerateLine(seed, dest, thick);
    }
    else 
    {
        // generate circle
        const int radius = rand() % (circle_radius_max - circle_radius_min);
        return ShapeGenerator::GenerateCircle(seed, radius);
    }
}

void PheromoneParams::applyCoefficients(unsigned to_food, unsigned from_food, 
    unsigned anthill)
{
    // 0.01 because GUI has 1-100 knobs
    to_food_decay_rate = 0.01 * float(to_food) * scale;
    from_food_decay_rate = 0.01 * float(from_food) * scale;
    anthill_decay_rate = 0.01 * float(anthill) * scale;
}    

void AntsParams::applyNumAnts(unsigned desired_num_ants)
{
    quantity = desired_num_ants;
}

void AntsParams::applyRatioScouts(unsigned percent_scouts)
{
    ratio_scouts = float(percent_scouts) / 100.0;
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