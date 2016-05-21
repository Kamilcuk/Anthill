#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_DYN_LINK
#endif

#include <boost/test/unit_test.hpp>

#include "../simulation/world.hpp"
#include "../simulation/updatable.hpp"

namespace UpdatableTest
{

struct Fixture 
{
    World world;
    
    struct ConcreteUpdatable : public Updatable
    {
        ConcreteUpdatable(World* w) : Updatable(w) {}
        virtual void step(int) override {}
    };
};

BOOST_FIXTURE_TEST_CASE(
    test_createUpdatable_shouldBeAddedAndRemovedUponConstructionDestruction, 
    Fixture)
{
    BOOST_CHECK_EQUAL(world.getUpdatablePtrs().size(), 0);
    {
        ConcreteUpdatable u(&world);
        BOOST_CHECK_EQUAL(world.getUpdatablePtrs().size(), 1);
    }
    BOOST_CHECK_EQUAL(world.getUpdatablePtrs().size(), 0);
}

BOOST_FIXTURE_TEST_CASE(test_operators, Fixture)
{
    ConcreteUpdatable u1(&world);
    ConcreteUpdatable u2(u1);
    BOOST_CHECK_EQUAL(world.getUpdatablePtrs().size(), 2);
}
    
} // namespace UpdatableTest