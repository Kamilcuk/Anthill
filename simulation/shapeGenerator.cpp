#include "shapeGenerator.hpp"

#include <math.h>

#include "point.hpp"

std::set<Point> ShapeGenerator::GenerateLine(const Point& start, const Point& end, unsigned thickness)
{
	float horizontal_change = (end.posX() - start.posX());
	float vertical_change = (end.posY() - start.posY());

	float len = sqrt(horizontal_change*horizontal_change + vertical_change*vertical_change);

	float dx = horizontal_change / len, dy = vertical_change / len;

	float x = start.posX(), y = start.posY();

	std::set<Point> points_on_line;

	while(! ((int)x == end.posX() && (int)y == end.posY())) // repeat until end point reached
	{
		std::set<Point> points_in_circle = GenerateCircle(Point(x, y), (unsigned)((float)thickness / 2.0));
		
		for(auto point : points_in_circle)
			points_on_line.insert(point);

		x += dx;
		y += dy;
	}

	return points_on_line;
}

std::set<Point> ShapeGenerator::GenerateCircle(const Point& centre, unsigned radius)
{
	std::set<Point> points_in_circle;

	for(unsigned x = centre.posX() - radius; x < centre.posX() + radius; x++)
		for(unsigned y = centre.posY() - radius; y < centre.posY() + radius; y++)
		{
			const signed dx = (signed)centre.posX() - (signed)x, dy = (signed)centre.posY() - (signed)y;
			const float dist_from_center = sqrt(dx*dx + dy*dy);

			if(dist_from_center <= (float)radius)
				points_in_circle.insert(Point(x, y));
		}

	return points_in_circle;
}
