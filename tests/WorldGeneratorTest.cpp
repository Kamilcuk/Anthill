#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_DYN_LINK
#endif

#include <boost/test/unit_test.hpp>

#include <stdlib.h>
#include <time.h>

#include <boost/make_shared.hpp>

#include "../simulation/world.hpp"
#include "../simulation/worldGenerator.hpp"
#include "../simulation/anthill.hpp"
#include "../simulation/ant.hpp"

namespace WorldGeneratorTest
{
    
struct Fixture 
{
    Fixture() { srand(time(NULL)); }
    World world;
};    

BOOST_FIXTURE_TEST_CASE(test_placeAnthill_shouldBeAnthill, Fixture)
{
    BOOST_CHECK_EQUAL(world.getSimulationObjects<Anthill>().size(), 0);
    AnthillParams params;
    params.quantity = 1;    
    WorldGenerator::placeAnthill(&world, params);
    BOOST_CHECK_EQUAL(world.getSimulationObjects<Anthill>().size(), 1);
    BOOST_CHECK_EQUAL(world.getEntityPtrs().size(), 1);
}

BOOST_FIXTURE_TEST_CASE(test_placeAnts_shouldBeAnts, Fixture)
{
    WorldGenerator::placeAnthill(&world);    
    
    BOOST_CHECK_EQUAL(world.getSimulationObjects<Creature>().size(), 0);    
    AntsParams params;
    params.quantity = 10;
    params.min_dist_from_anthill = 2;
    params.max_dist_from_anthill = 5;
    WorldGenerator::placeAnts(&world, params);
    BOOST_CHECK_EQUAL(world.getSimulationObjects<Creature>().size(), 10);
    BOOST_CHECK_EQUAL(world.getEntityPtrs().size(), 11); // (+ anthill)
}

BOOST_FIXTURE_TEST_CASE(test_placeObstacles_shouldBeObstacles, Fixture)
{
    BOOST_CHECK_EQUAL(world.getSimulationObjects<Obstacle>().size(), 0);    
    ObstaclesParams params;
    params.quantity = 5;
    WorldGenerator::placeObstacles(&world, params);
    BOOST_CHECK(world.getSimulationObjects<Obstacle>().size() > 0);
    BOOST_CHECK(world.getEntityPtrs().size() > 0);
}

BOOST_FIXTURE_TEST_CASE(test_placeFoods_shouldBeFoods, Fixture)
{
    BOOST_CHECK_EQUAL(world.getSimulationObjects<Food>().size(), 0);    
    FoodsParams params;
    params.quantity = 5;
    WorldGenerator::placeFoods(&world, params);
    BOOST_CHECK(world.getSimulationObjects<Food>().size() > 0);
    BOOST_CHECK(world.getEntityPtrs().size() > 0);
}

} // namespace WorldGeneratorTest