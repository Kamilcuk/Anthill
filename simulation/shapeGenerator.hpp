#ifndef SHAPE_GENERATOR_H
#define SHAPE_GENERATOR_H

#include <set>

class Point;

/**
 * Helper class for generating formations of points on a 2D grid.
 */
class ShapeGenerator
{
public:
	std::set<Point> GenerateLine(const Point& start, const Point& end, unsigned thickness);
	std::set<Point> GenerateCircle(const Point& centre, unsigned radius);
};

#endif // SHAPE_GENERATOR_H