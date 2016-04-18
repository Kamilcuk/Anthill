#ifndef BOST_TEST_DYN_LINK
#define BOOST_TEST_DYN_LINK
#endif

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include <iostream>

#include "../simulation/world.hpp"
#include "../simulation/pheromoneMap.hpp"
#include "../simulation/point.hpp"

const float g_epsilon = 0.0001;

struct Fixture 
{
    const unsigned size_x, size_y;
    const float decay_coef;
    World world;
    PheromoneMap pheromone_map;
 
    Fixture() :
        size_x(10), size_y(10), decay_coef(1), world(),
        pheromone_map(world, PheromoneMap::Type::ToFood, size_x, size_y, decay_coef)
    {
        BOOST_TEST_MESSAGE("setting up pheromone map test");
    }
    
    ~Fixture()
    {
        BOOST_TEST_MESSAGE("tearing down pheromone map test");
    }
};

BOOST_FIXTURE_TEST_CASE(test_resetMap, Fixture)
{
    const float test_strength = 10.0;
    const float radius_one = 1.0;
    pheromone_map.createBlob(Point(5, 5), radius_one, test_strength);   
    BOOST_CHECK_CLOSE(pheromone_map.getStrengthAtPosition(Point(5, 5)),
        test_strength, g_epsilon);
        
    pheromone_map.reset();
    BOOST_CHECK_CLOSE(pheromone_map.getStrengthAtPosition(Point(5, 5)),
        0.0, g_epsilon);
}

BOOST_FIXTURE_TEST_CASE(test_create1SquareBlob, Fixture)
{
    const float test_strength = 10.0;
    const float radius_one = 1.0;
    pheromone_map.createBlob(Point(5, 5), radius_one, test_strength);   
    BOOST_CHECK_CLOSE(pheromone_map.getStrengthAtPosition(Point(5, 5)),
        test_strength, g_epsilon);
    BOOST_CHECK_CLOSE(pheromone_map.getStrengthAtPosition(Point(6, 5)),
        0.0, g_epsilon);
}

BOOST_FIXTURE_TEST_CASE(test_createBlob, Fixture)
{     
    const float test_strength = 10.0;    
    const float radius_positive = 5.0;
    pheromone_map.createBlob(Point(9, 9), radius_positive, test_strength);   
    BOOST_CHECK_CLOSE(pheromone_map.getStrengthAtPosition(Point(9, 9)),
        test_strength, g_epsilon);
    BOOST_CHECK_LE(pheromone_map.getStrengthAtPosition(Point(6, 6)),
        test_strength);
}