#include "point.hpp"
#include <iostream>

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

void Point::move(int addX, int addY) {
	posX_ += addX;
	posY_ += addY;
}

void Point::printPosition()
{
	std::cout << "PosX: " << posX() << " posY: " << posY() << std::endl;
}

Point Point::getPosition() const
{
	return Point(*this);
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
