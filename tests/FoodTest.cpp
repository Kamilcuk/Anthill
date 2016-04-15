#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../simulation/food.hpp"
#include "../simulation/world.hpp"

BOOST_AUTO_TEST_CASE( getSmell )
{
    World w;
    Food f(w, Point(0,0));
    BOOST_CHECK_EQUAL( f.getSmell(), 100 );
    BOOST_CHECK_EQUAL( f.getSmell(), 90 );
}


