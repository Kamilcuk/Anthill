#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_DYN_LINK
#endif

#include <boost/test/unit_test.hpp>

#include "../simulation/world.hpp"
#include "../simulation/worldGenerator.hpp"
#include "../simulation/painter.hpp"
#include "../simulation/pheromoneMap.hpp"
#include "../simulation/entity.hpp"
#include "../simulation/obstacle.hpp"
#include "../simulation/food.hpp"
#include "../simulation/point.hpp"

namespace PainterTest
{

struct Fixture 
{
    World world;
    Fixture() 
    {
        world.setDimensions(20, 20);
    }
};

BOOST_FIXTURE_TEST_CASE(test_drawEntities, Fixture)
{
    Painter::drawEntitiesWrapper<Obstacle>(&world, 5, 5, 5, 10, 1);
    BOOST_CHECK_GT(world.getSimulationObjects<Obstacle>().size(), 0);
    Painter::drawEntitiesWrapper<Food>(&world, 15, 15, 15, 10, 1);
    BOOST_CHECK_GT(world.getSimulationObjects<Food>().size(), 0);
}

BOOST_FIXTURE_TEST_CASE(
    test_drawEntitiesOnTopOfEachOther_shouldNotOverlap, Fixture)
{
    Painter::drawEntitiesWrapper<Obstacle>(&world, 5, 5, 5, 10, 5);
    BOOST_CHECK_GT(world.getSimulationObjects<Obstacle>().size(), 0);
    Painter::drawEntitiesWrapper<Food>(&world, 5, 5, 5, 10, 5);
    BOOST_CHECK_EQUAL(world.getSimulationObjects<Food>().size(), 0);
}

BOOST_FIXTURE_TEST_CASE(test_drawEntitiesCross_shouldNotOverlap, Fixture)
{
    Painter::drawEntitiesWrapper<Obstacle>(&world, 10, 0, 10, 20, 1);
    Painter::drawEntitiesWrapper<Food>(&world, 0, 10, 20, 10, 1);
    BOOST_CHECK_GT(world.getSimulationObjects<Obstacle>().size(), 
        world.getSimulationObjects<Food>().size());
}

BOOST_FIXTURE_TEST_CASE(test_drawEntitiesOutsideMap_shouldTrimThem, Fixture)
{
    Painter::drawEntitiesWrapper<Obstacle>(&world, 5, 5, 25, 10, 1);
    BOOST_CHECK_EQUAL(world.getSimulationObjects<Obstacle>().size(), 15);
}

BOOST_FIXTURE_TEST_CASE(test_drawPheromones, Fixture)
{
    WorldGenerator::initPheromoneMaps(&world);
        
    auto maps = world.getSimulationObjects<PheromoneMap>();
    
    auto to_food_map = *std::find_if(maps.begin(), maps.end(), 
        [] (auto p) -> bool { 
            return p->getType() == PheromoneMap::Type::ToFood; });
    auto from_food_map = *std::find_if(maps.begin(), maps.end(), 
        [] (auto p) -> bool { 
            return p->getType() == PheromoneMap::Type::FromFood; });
            
    const float strength = 5;
    const float radius = 15;
    
    Painter::drawPheromoneToFoodWrapper(&world, 5, 5, 5, 10, radius, strength);
    BOOST_CHECK_GT(to_food_map->getStrengthAtPosition(Point(3, 3)), 0);
        
    Painter::drawPheromoneFromFoodWrapper(&world, 5, 5, 5, 10, radius, strength);
    BOOST_CHECK_GT(from_food_map->getStrengthAtPosition(Point(3, 3)), 0);
}

} // namespace PainterTest