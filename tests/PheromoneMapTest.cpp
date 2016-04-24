#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_DYN_LINK
#endif

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include "../simulation/world.hpp"
#include "../simulation/pheromoneMap.hpp"
#include "../simulation/point.hpp"

namespace PheromoneMapTest
{
    
const float g_epsilon = 0.0001;

struct Fixture 
{
    const unsigned size_x, size_y;
    const float decay_coef;
    World world;
    PheromoneMap pheromone_map;

    Fixture() :
        size_x(10), size_y(10), decay_coef(1), world(),
        pheromone_map(&world, PheromoneMap::Type::ToFood, size_x, size_y, decay_coef)
    {}
};

BOOST_FIXTURE_TEST_CASE(test_getInvalidPoint_shouldThrow, Fixture)
{
    BOOST_CHECK_THROW(pheromone_map.getStrengthAtPosition(Point(-5, -5)),
        std::exception);
    BOOST_CHECK_THROW(pheromone_map.getStrengthAtPosition(Point(size_x+1, size_y+1)),
        std::exception);
}

BOOST_FIXTURE_TEST_CASE(test_resetMap_shouldBeZeroEverywhere, Fixture)
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

BOOST_FIXTURE_TEST_CASE(test_create1SquareBlob_shouldBeOnly1SquareWide, Fixture)
{
    const float test_strength = 10.0;
    const float radius_one = 1.0;
    pheromone_map.createBlob(Point(5, 5), radius_one, test_strength);   
    BOOST_CHECK_CLOSE(pheromone_map.getStrengthAtPosition(Point(5, 5)),
        test_strength, g_epsilon);
    BOOST_CHECK_CLOSE(pheromone_map.getStrengthAtPosition(Point(6, 5)),
        0.0, g_epsilon);
}

BOOST_FIXTURE_TEST_CASE(test_createBlobExceedBoundary_shouldClipNoExceptions, Fixture)
{
    const float test_strength = 10.0;
    const float radius_one = 5.0;
    BOOST_CHECK_NO_THROW(pheromone_map.createBlob(Point(9, 9), radius_one, test_strength));
}

BOOST_FIXTURE_TEST_CASE(test_createBlob_shouldDropOffWithDistance, Fixture)
{     
    const float test_strength = 10.0;    
    const float radius_positive = 5.0;
    pheromone_map.createBlob(Point(9, 9), radius_positive, test_strength);   
    BOOST_CHECK_CLOSE(pheromone_map.getStrengthAtPosition(Point(9, 9)),
        test_strength, g_epsilon);
    BOOST_CHECK_LE(pheromone_map.getStrengthAtPosition(Point(6, 6)),
        test_strength);
}

BOOST_FIXTURE_TEST_CASE(test_decay_shouldDropWithTime, Fixture)
{     
    const float test_strength = 10.0;    
    const float radius_positive = 5.0;
    pheromone_map.createBlob(Point(5, 5), radius_positive, test_strength);   
    BOOST_CHECK_CLOSE(pheromone_map.getStrengthAtPosition(Point(5, 5)),
        test_strength, g_epsilon);
    pheromone_map.step(1);
    pheromone_map.step(1);
    pheromone_map.step(1);
    pheromone_map.step(1);
    BOOST_CHECK_LE(pheromone_map.getStrengthAtPosition(Point(5, 5)),
        test_strength);
}

} // namespace PheromoneMapTest