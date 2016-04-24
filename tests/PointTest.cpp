#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_DYN_LINK
#endif

#include <boost/test/unit_test.hpp>

#include "../simulation/point.hpp"

BOOST_AUTO_TEST_CASE( position )
{
    Point p(0,0);
    BOOST_CHECK_EQUAL( p.posX(), 0 );
    BOOST_CHECK_EQUAL( p.posY(), 0 );

    Point q(123,456);
    BOOST_CHECK_EQUAL( q.posX(), 123 );
    BOOST_CHECK_EQUAL( q.posY(), 456 );

    BOOST_CHECK( q!=p );

    p.setPosY(q.posY());
    BOOST_CHECK_EQUAL( p.posX(), 0 );
    BOOST_CHECK_EQUAL( p.posY(), 456 );

    p.setPosX(q.posX());
    BOOST_CHECK_EQUAL( p.posX(), 123 );
    BOOST_CHECK_EQUAL( p.posY(), 456 );

    BOOST_CHECK( q==p );
}
