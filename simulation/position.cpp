#include "position.hpp"
#include <iostream>

int Position::posX() const
{
	return posX_;
}

void Position::setPosX(int posX)
{
	posX_ = posX;
}

int Position::posY() const
{
	return posY_;
}

void Position::setPosY(int posY)
{
	posY_ = posY;
}

void Position::move(int addX, int addY) {
	posX_ += addX;
	posY_ += addY;
}

void Position::printPosition()
{
	std::cout << "PosX: " << posX() << " posY: " << posY() << std::endl;
}

Position Position::getPosition() const
{
	return Position(*this);
}

Position::Position(int posX, int posY) :
	posX_(posX),
	posY_(posY)
{

}

Position::Position(const Position &pos)
{
	posX_ = pos.posX();
	posY_ = pos.posY();
}
