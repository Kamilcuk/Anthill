#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_DYN_LINK
#endif

#include <boost/test/unit_test.hpp>

#include <boost/make_shared.hpp>

#include "../simulation/world.hpp"
#include "../simulation/entity2DMap.hpp"
#include "../simulation/entity.hpp"
#include "../simulation/obstacle.hpp"
#include "../simulation/food.hpp"
#include "../simulation/point.hpp"

namespace Entity2DMapTest
{

struct Fixture 
{
    World world;
    Fixture() 
    {
        world.setDimensions(20, 20);
    }
};

BOOST_FIXTURE_TEST_CASE(test_getEntitiesInSquare, Fixture)
{
    world.addSimulationObject<Obstacle>(
        boost::make_shared<Obstacle>(&world, Point(10, 10)));
    world.addSimulationObject<Obstacle>(
        boost::make_shared<Obstacle>(&world, Point(15, 10)));
    world.addSimulationObject<Obstacle>(
        boost::make_shared<Obstacle>(&world, Point(12, 15)));
    world.addSimulationObject<Obstacle>(
        boost::make_shared<Obstacle>(&world, Point(2, 3)));
        
    BOOST_CHECK_EQUAL(world.getEntityMap().lock()->getAllEntities().size(), 4);
    BOOST_CHECK_EQUAL(world.getEntityMap().lock()->
        getEntitiesInSquare(Point(10, 10), Point(20, 20)).size(), 3);
}


} // namespace Entity2DMapTest