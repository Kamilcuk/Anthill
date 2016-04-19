#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_DYN_LINK
#endif

#include <boost/test/unit_test.hpp>

#include <set>
#include <algorithm>

#include "../simulation/shapeGenerator.hpp"
#include "../simulation/point.hpp"

const float g_epsilon = 0.0001;

bool FindPoint(int x, int y, std::set<Point>& set)
{
    return std::find_if(set.begin(), set.end(), [x, y](Point p)->bool
        { return (p.posX() == x && p.posY() == y); }) != set.end();
}

BOOST_AUTO_TEST_CASE(test_generateLineThin_shouldStayInBoundries)
{
    const Point start(2, 2), end(2, 15);
    const unsigned thickness = 0;
    
    ShapeGenerator shape_gen;    
    auto output = shape_gen.GenerateLine(start, end, thickness);
    
    BOOST_CHECK(FindPoint(2, 2, output));
    BOOST_CHECK(FindPoint(2, 10, output));
    BOOST_CHECK(FindPoint(2, 15, output));
    BOOST_CHECK(!FindPoint(1, 10, output));
    BOOST_CHECK(!FindPoint(3, 10, output));
    BOOST_CHECK(!FindPoint(2, 1, output));
    BOOST_CHECK(!FindPoint(2, 16, output));
}

BOOST_AUTO_TEST_CASE(test_invalidParams_shouldThrow)
{
    const Point point(1, 1);
    const int thickness = -1, radius = -1;
    ShapeGenerator shape_gen;    
    BOOST_CHECK_THROW(shape_gen.GenerateLine(point, point, thickness),
        std::exception);
    BOOST_CHECK_THROW(shape_gen.GenerateCircle(point, radius),
        std::exception);
}
    
BOOST_AUTO_TEST_CASE(test_generateLine_shouldHaveCorrectEnds)
{
    const Point start(2, 2), end(5, 15);
    const unsigned thickness = 0;
    
    ShapeGenerator shape_gen;    
    auto output = shape_gen.GenerateLine(start, end, thickness);
    
    BOOST_CHECK(FindPoint(2, 2, output));
    BOOST_CHECK(FindPoint(5, 15, output));
}

BOOST_AUTO_TEST_CASE(test_generateLine_shouldStayInBoundries)
{
    const Point start(2, 2), end(2, 15);
    const unsigned thickness = 3;
    
    ShapeGenerator shape_gen;    
    auto output = shape_gen.GenerateLine(start, end, thickness);
    
    BOOST_CHECK(FindPoint(2, 2, output));
    BOOST_CHECK(FindPoint(2, 10, output));
    BOOST_CHECK(FindPoint(2, 15, output));
    BOOST_CHECK(FindPoint(3, 10, output));
    BOOST_CHECK(!FindPoint(4, 10, output));
    BOOST_CHECK(FindPoint(2, 16, output));
    BOOST_CHECK(!FindPoint(2, 17, output));
}

BOOST_AUTO_TEST_CASE(test_lineReverseStartEnd_shouldBeSimilarToNotReversed)
{
    const Point start(2, 2), end(5, 15);
    const unsigned thickness = 1;
    
    ShapeGenerator shape_gen;    
    auto output = shape_gen.GenerateLine(start, end, thickness);
    auto output_reversed = shape_gen.GenerateLine(end, start, thickness);
    
    BOOST_CHECK(FindPoint(2, 2, output));
    BOOST_CHECK(FindPoint(5, 15, output));
    BOOST_CHECK(FindPoint(2, 2, output_reversed));
    BOOST_CHECK(FindPoint(5, 15, output_reversed));
    BOOST_CHECK(output.size() == output_reversed.size());
}

BOOST_AUTO_TEST_CASE(test_generateOneSquare_shouldBeOnlyOneSquare)
{
    const Point centre(5, 5);
    const unsigned radius = 0;
    
    ShapeGenerator shape_gen;    
    auto output = shape_gen.GenerateCircle(centre, radius);
    
    BOOST_CHECK(FindPoint(5, 5, output));
    BOOST_CHECK(!FindPoint(4, 5, output));
    BOOST_CHECK(!FindPoint(3, 5, output));
    BOOST_CHECK(!FindPoint(2, 5, output));
    BOOST_CHECK(!FindPoint(1, 5, output));
}

BOOST_AUTO_TEST_CASE(test_generateCircle_shouldBeCorrectRadius)
{
    const Point centre(5, 5);
    const unsigned radius = 2;
    
    ShapeGenerator shape_gen;    
    auto output = shape_gen.GenerateCircle(centre, radius);
    
    BOOST_CHECK(FindPoint(5, 5, output));
    BOOST_CHECK(FindPoint(4, 5, output));
    BOOST_CHECK(FindPoint(3, 5, output));
    BOOST_CHECK(!FindPoint(2, 5, output));
    BOOST_CHECK(!FindPoint(1, 5, output));
}