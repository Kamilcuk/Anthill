#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_DYN_LINK
#endif

#include <boost/test/unit_test.hpp>
#include <boost/make_shared.hpp>

#include "../simulation/food.hpp"
#include "../simulation/world.hpp"

namespace FoodTest
{

BOOST_AUTO_TEST_CASE( testUsed )
{
    {
        World w;
        Food f(&w, Point(0,0));

        BOOST_CHECK_EQUAL(f.getUsed(), false);
        f.setUsed(1);
        BOOST_CHECK_EQUAL(f.getUsed(), true);
    }
    {
        World w;
        auto p=boost::make_shared<Food>(&w,Point(30,40));
        BOOST_CHECK_EQUAL(p->getUsed(), false);

        auto wp=boost::weak_ptr<Food>(p);
        BOOST_CHECK_EQUAL(wp.expired(), false);
        p.reset();
        BOOST_CHECK_EQUAL(wp.expired(), true);

        w.simulationStep();
        BOOST_CHECK_EQUAL(wp.expired(), true);
    }
    
}

} // namespace FoodTest