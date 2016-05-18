#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_DYN_LINK
#endif

#include <boost/test/unit_test.hpp>

#include "../simulation/world.hpp"
#include "../simulation/entity.hpp"
#include "../simulation/point.hpp"

namespace EntityTest
{

struct Fixture 
{
    World world;
};

BOOST_FIXTURE_TEST_CASE(test_step_shouldThrow, Fixture)
{
    Entity e(&world);
    BOOST_CHECK_THROW(e.step(1), std::runtime_error);
}

BOOST_FIXTURE_TEST_CASE(test_construct_shouldBeInUpdatablePtrs, Fixture)
{
    Entity e(&world);
    BOOST_CHECK(world.getUpdatablePtrs()[0] == &e);
}
    
} // namespace EntityTest