#include "point.hpp"
#include <iostream>
#include <cmath>

void Point::setPosX(int posX)
{
	posX_ = posX;
}

void Point::setPosY(int posY)
{
	posY_ = posY;
}

std::string Point::toString() const
{
	std::string s;
	s += std::to_string(posX_);
	s += ",";
	s += std::to_string(posY_);
	return s;
}

std::ostream& operator<<(std::ostream &os, const Point& rhs)
{
	os << rhs.toString();
	return os;
}