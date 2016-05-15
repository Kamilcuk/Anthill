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
    static void drawEntities(World* world, const Point& from, 
        const Point& to)
    {
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
     
        
        // remember only positions of entities in a rectangle defined by 
        // "from" and "to" params, to improve performance
        const auto& all_entities = world->getEntityPtrs();
        std::set<Point> nonempty_spaces_trimmed;
        
        std::for_each(all_entities.begin(), all_entities.end(),
            [&nonempty_spaces_trimmed, &from, &to] (auto entity) -> void { 
                if(entity.lock()->getPos().isInBounds(from, to))
                    nonempty_spaces_trimmed.insert(entity.lock()->getPos()); 
            }
        );
        
        // remove painted points for which their space is already occupied
        candidate_points.erase(
            std::remove_if(candidate_points.begin(), candidate_points.end(),
                [&nonempty_spaces_trimmed] (auto candidate_pt) -> bool {
                    for(auto& nonempty_space : nonempty_spaces_trimmed)
                        if(candidate_pt == nonempty_space)
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
    
    /// Increases strength of specified pheromone in blobs in a line specified
    /// by params "from" and "to".
    static void drawPheromone(World* world, const Point& from, 
        const Point& to, PheromoneMap::Type type)
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
        const int radius = 3;
        const float strength = 50;
        for(const auto& valid_point : candidate_points)
            our_map->createBlob(valid_point, radius, strength);
    }
    

    /// Wrapper for drawPheromone (for easier Python export)
    static void drawPheromoneToFoodWrapper(World* world, const int from_x, 
        const int from_y, const int to_x, const int to_y)
    {
        drawPheromone(world, Point(from_x, from_y), Point(to_x, to_y),
            PheromoneMap::Type::ToFood);
    }
    
    /// Wrapper for drawPheromone (for easier Python export)
    static void drawPheromoneFromFoodWrapper(World* world, const int from_x, 
        const int from_y, const int to_x, const int to_y)
    {
        drawPheromone(world, Point(from_x, from_y), Point(to_x, to_y),
            PheromoneMap::Type::FromFood);
    }
    
    /// Wrapper for drawEntities (for easier Python export)
    template<class T>
    static void drawEntitiesWrapper(World* world, const int from_x, 
        const int from_y, const int to_x, const int to_y)
    {
        Painter::drawEntities<T>(world, Point(from_x, from_y), 
            Point(to_x, to_y));
    }
};

#endif // PAINTER_H