#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>


#include "../simulation/point.hpp"

BOOST_AUTO_TEST_CASE( position )
{
    Point p(0,0);
    BOOST_CHECK_EQUAL( p.posX(), 0 );
    BOOST_CHECK_EQUAL( p.posY(), 90 );
}
