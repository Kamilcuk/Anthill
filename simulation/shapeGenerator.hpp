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

	/// Returns a set of points which form a line from start to end of 
	///	specified thickness
	std::set<Point> GenerateLine(const Point& start, const Point& end, 
		int thickness);
	
	/// Returns a set of points that form a filled circle (including edge)
	std::set<Point> GenerateCircle(const Point& centre, int radius);
};

#endif // SHAPE_GENERATOR_H