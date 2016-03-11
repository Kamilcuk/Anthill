#ifndef POSITION_H
#define POSITION_H


class Point
{
	int posX_;
	int posY_;
public:
	Point(int posX, int posY);
	Point(const Point& pos);
	int posX() const;
	void setPosX(int posX);
	int posY() const;
	void setPosY(int posY);
	Point move(int addX, int addY);
	void printPosition();
	Point getPosition() const;
	bool operator<(const Point& rhs) const { return (posX() < rhs.posX()) || (posY() < rhs.posY()); } // for std::set
};

#endif // POSITION_H
