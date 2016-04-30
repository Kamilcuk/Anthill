#include "point.hpp"
#include <iostream>
#include <cmath>

int Point::posX() const
{
	return posX_;
}

void Point::setPosX(int posX)
{
	posX_ = posX;
}

int Point::posY() const
{
	return posY_;
}

void Point::setPosY(int posY)
{
	posY_ = posY;
}

void Point::printPosition()
{
	std::cout << "PosX: " << posX() << " posY: " << posY() << std::endl;
}

Point Point::getPosition() const
{
    return Point(*this);
}

float Point::getDistance(Point p) const
{
        return std::sqrt(
                std::pow(this->posX() - p.posX(), 2) +
                std::pow(this->posY() - p.posY(), 2)
                );

}

Point::Point(int posX, int posY) :
	posX_(posX),
	posY_(posY)
{

}

Point::Point(const Point &pos)
{
	posX_ = pos.posX();
	posY_ = pos.posY();
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