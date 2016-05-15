#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_DYN_LINK
#endif

#include <sstream>

#include <boost/test/unit_test.hpp>

#include "../simulation/point.hpp"

namespace PointTest
{

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
    
    BOOST_CHECK_EQUAL(p.isInBounds(Point(15, 15)), true);
    BOOST_CHECK_EQUAL(p.isInBounds(Point(5, 15)), false);
    BOOST_CHECK_EQUAL(p.isInBounds(Point(15, 5)), false);
    BOOST_CHECK_EQUAL(p.isInBounds(Point(5, 5)), false);
    
    BOOST_CHECK_EQUAL(p.isInBounds(Point(5, 5), Point(15, 15)), true);
    BOOST_CHECK_EQUAL(p.isInBounds(Point(13, 5), Point(15, 15)), false);
    BOOST_CHECK_EQUAL(p.isInBounds(Point(5, 13), Point(15, 15)), false);
    BOOST_CHECK_EQUAL(p.isInBounds(Point(5, 5), Point(8, 15)), false);
    BOOST_CHECK_EQUAL(p.isInBounds(Point(5, 5), Point(15, 8)), false);
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

BOOST_AUTO_TEST_CASE(test_distance)
{
    Point p(2, 2), q(4, 2);
    BOOST_CHECK_CLOSE(p.getDistance(q), 2, 0.00001); 
}

BOOST_AUTO_TEST_CASE(test_toString)
{
    Point p(2, 2);
    BOOST_CHECK_EQUAL(p.toString(), "2,2");
}

BOOST_AUTO_TEST_CASE(test_toStringStream)
{
    Point p(2, 2);
    std::stringstream test_stream;
    test_stream << p;
    BOOST_CHECK_EQUAL(test_stream.str(), "2,2");
}

} // namespace PointTest