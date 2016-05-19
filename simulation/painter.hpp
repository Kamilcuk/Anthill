#ifndef PAINTER_H
#define PAINTER_H

#include <set>
#include <algorithm>

#include <boost/make_shared.hpp>

#include "point.hpp"
#include "shapeGenerator.hpp"
#include "pheromoneMap.hpp"

/**
 * Utility class for painting simulation objects onto the map using mouse 
 * pointer (wrapper for external Python GUI)
 */
class Painter
{
public:

    /// Spawns entities of specified type between two supplied positions. 
    /// Doesn't overwrite spaces in which something already exists.
    template<class T>
    inline static void drawEntities(World* world, const Point& from, 
        const Point& to, const float radius, const float strength)
    {
        auto points_set = ShapeGenerator::GenerateLine(from, to, radius);
        
        // copy from set to vector, because it's easier to remove elements
        // from a vector.
        std::vector<Point> candidate_points;
        std::copy(points_set.begin(), points_set.end(), 
            std::back_inserter(candidate_points));
            
        // remove points outside of the map
        candidate_points.erase(
            std::remove_if(candidate_points.begin(), candidate_points.end(),
                [world] (auto candidate_pt) -> bool { 
                return !(candidate_pt.isInBounds(world->getDimensions())); 
            }),
            candidate_points.end()
        );
     
        
        const int leftmost = std::min(from.posX(), to.posX()) - radius,
            rightmost = std::max(from.posX(), to.posX()) + radius,
            bottommost = std::min(from.posY(), to.posY()) - radius,
            topmost = std::max(from.posY(), to.posY()) + radius;
        const Point botleft(leftmost, bottommost), topright(rightmost, topmost);
        
        const auto& entities = world->getEntityMap().lock()->
            getEntitiesInSquare(botleft, topright);

        // remove painted points for which their space is already occupied
        candidate_points.erase(
            std::remove_if(candidate_points.begin(), candidate_points.end(),
                [&entities] (auto candidate_pt) -> bool {
                    for(auto& e : entities)
                        if(candidate_pt == e.lock()->getPos())
                            return true;
                    return false;
                }),
            candidate_points.end()
        );
        
        // make selected entities at valid locations
        for(const auto& valid_point : candidate_points)
            world->addSimulationObject<T>(boost::make_shared<T>(
                world, valid_point));
    }
    
    /// Increases strength of specified pheromone in blobs of specified radius
    /// and of specified central strength, in a line specified
    /// by params "from" and "to".
    inline static void drawPheromone(World* world, const Point& from, 
        const Point& to, PheromoneMap::Type type, const float radius, 
        const float strength)
    {
        auto& pheromone_maps = world->getSimulationObjects<PheromoneMap>();
        
        boost::shared_ptr<PheromoneMap> our_map;
        for(const auto& map : pheromone_maps)
            if(map->getType() == type)
            {
                our_map = map;
                break;
            }
        
        const int thick = 1;
        auto points_set = ShapeGenerator::GenerateLine(from, to, thick);
        
        // copy from set to vector, because it's easier to remove elements
        // from a vector.
        std::vector<Point> candidate_points;
        std::copy(points_set.begin(), points_set.end(), 
            std::back_inserter(candidate_points));
            
        // remove points outside of the map
        candidate_points.erase(
            std::remove_if(candidate_points.begin(), candidate_points.end(),
                [world] (auto candidate_pt) -> bool { 
                return !(candidate_pt.isInBounds(world->getDimensions())); 
            }),
            candidate_points.end()
        );
        
        // create blobs in valid points
        for(const auto& valid_point : candidate_points)
            our_map->createBlob(valid_point, radius, strength);
    }
    

    /// Wrapper for drawPheromone (for easier Python export)
    inline static void drawPheromoneToFoodWrapper(World* world, 
        const int from_x, const int from_y, const int to_x, const int to_y,
        const float radius, const float strength)
    {
        drawPheromone(world, Point(from_x, from_y), Point(to_x, to_y),
            PheromoneMap::Type::ToFood, radius, strength);
    }
    
    /// Wrapper for drawPheromone (for easier Python export)
    inline static void drawPheromoneFromFoodWrapper(World* world, 
        const int from_x, const int from_y, const int to_x, const int to_y,
        const float radius, const float strength)
    {
        drawPheromone(world, Point(from_x, from_y), Point(to_x, to_y),
            PheromoneMap::Type::FromFood, radius, strength);
    }
    
    /// Wrapper for drawEntities (for easier Python export)
    template<class T>
    inline static void drawEntitiesWrapper(World* world, const int from_x, 
        const int from_y, const int to_x, const int to_y,
        const float radius, const float strength = 0)
    {
        Painter::drawEntities<T>(world, Point(from_x, from_y), 
            Point(to_x, to_y), radius, strength);
    }
};

#endif // PAINTER_H