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
    BOOST_CHECK_EQUAL(world.getAnthills().size(), 0);
    AnthillParams params;
    params.quantity = 1;    
    WorldGenerator::placeAnthill(&world, params);
    BOOST_CHECK_EQUAL(world.getAnthills().size(), 1);
    BOOST_CHECK_EQUAL(world.getEntityPtrs().size(), 1);
}

BOOST_FIXTURE_TEST_CASE(test_placeAnts_shouldBeAnts, Fixture)
{
    WorldGenerator::placeAnthill(&world);    
    
    BOOST_CHECK_EQUAL(world.getAnts().size(), 0);    
    AntsParams params;
    params.quantity = 10;
    params.min_dist_from_anthill = 2;
    params.max_dist_from_anthill = 5;
    WorldGenerator::placeAnts(&world, params);
    BOOST_CHECK_EQUAL(world.getAnts().size(), 10);
    BOOST_CHECK_EQUAL(world.getEntityPtrs().size(), 11);
}

} // namespace WorldGeneratorTest