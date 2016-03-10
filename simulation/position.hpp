#ifndef POSITION_H
#define POSITION_H


class Position
{
	int posX_;
	int posY_;
public:
	Position(int posX, int posY);
	Position(const Position& pos);
	int posX() const;
	void setPosX(int posX);
	int posY() const;
	void setPosY(int posY);
	void move(int addX, int addY);
	void printPosition();
	Position getPosition() const;
};

#endif // POSITION_H
