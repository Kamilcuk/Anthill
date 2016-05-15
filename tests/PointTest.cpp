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

BOOST_AUTO_TEST_CASE(test_isInBounds)
{
    Point p(10, 10);
    BOOST_CHECK_EQUAL(p.isInBounds(15, 15), true);    
    BOOST_CHECK_EQUAL(p.isInBounds(11, 11), true);
    BOOST_CHECK_EQUAL(p.isInBounds(10, 10), false);
    BOOST_CHECK_EQUAL(p.isInBounds(15, 5), false);    
    BOOST_CHECK_EQUAL(p.isInBounds(5, 15), false);    
    BOOST_CHECK_EQUAL(p.isInBounds(5, 5), false);    
}

BOOST_AUTO_TEST_CASE(test_operators)
{
    Point p(2, 2), q(5, 5);
    BOOST_CHECK_EQUAL(Point(7, 7), p + q);
    BOOST_CHECK_EQUAL(Point(-3, -3), p - q);
    BOOST_CHECK_EQUAL(20, p * q);
    BOOST_CHECK_EQUAL(Point(6, 6), p * 3);
    
    p += q;
    BOOST_CHECK_EQUAL(Point(7, 7), p);
    q -= p;
    BOOST_CHECK_EQUAL(Point(-2, -2), q);
    
    p *= 2;
    BOOST_CHECK_EQUAL(Point(14, 14), p);
    
    BOOST_CHECK(p != q);
    BOOST_CHECK(!(p == q));
}