#include "shapeGenerator.hpp"

#include <algorithm>
#include <stdexcept>

#include "point.hpp"

std::set<Point> ShapeGenerator::GenerateLine(const Point& start, 
	const Point& end, int thickness)
{
	if(thickness < 0) throw std::runtime_error(
		"Line thickness can't be negative");
	
	const float horizontal_change = (end.posX() - start.posX()),
		vertical_change = (end.posY() - start.posY()),
		len = sqrt(horizontal_change*horizontal_change + 
			vertical_change*vertical_change),
		dx = horizontal_change / len, dy = vertical_change / len,
		boundary_left = std::min(start.posX(), end.posX()),
		boundary_right = std::max(start.posX(), end.posX()),
		boundary_top = std::max(start.posY(), end.posY()),
		boundary_bottom = std::min(start.posY(), end.posY());

	float x = start.posX(), y = start.posY();
	std::set<Point> points_on_line;

	while (x >= boundary_left && x <= boundary_right && y >= boundary_bottom 
		&& y <= boundary_top)
	{
		std::set<Point> points_in_circle = GenerateCircle(Point(std::round(x), 
			std::round(y)), (unsigned)((float)thickness / 2.0));
		
		for(auto point : points_in_circle)
			points_on_line.insert(point);

		x += dx;
		y += dy;
	}

	return points_on_line;
}


std::set<Point> ShapeGenerator::GenerateCircle(const Point& centre, int radius)
{
	if(radius < 0) throw std::runtime_error("Radius can't be negative");
	
	std::set<Point> points_in_circle;

	for(int x = centre.posX() - radius; x <= centre.posX() + radius; x++)
		for(int y = centre.posY() - radius; y <= centre.posY() + radius; y++)
		{
			const signed dx = (signed)centre.posX() - (signed)x, 
				dy = (signed)centre.posY() - (signed)y;
			const float dist_from_center = sqrt(dx*dx + dy*dy);

			if(dist_from_center <= (float)radius)
				points_in_circle.insert(Point(x, y));
		}

	return points_in_circle;
}
