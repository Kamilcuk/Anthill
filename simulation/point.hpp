#ifndef POINT_H
#define POINT_H

class Point
{
	int posX_;
	int posY_;
public:
    Point():posX_(0),posY_(0){}
	Point(int posX, int posY);
	Point(const Point& pos);
	
    Point& operator=(const Point&);
	
	bool operator<(const Point& rhs) const // for std::set
	{ return (posX() < rhs.posX()) || (posY() < rhs.posY()); } 
	
	bool operator!=(const Point& rhs) const 
	{ return (posX() != rhs.posX()) || (posY() != rhs.posY()); } 
	
	bool operator==(const Point& rhs) const 
	{ return !(*this != rhs); }
	
	int posX() const;
	int posY() const;
	
	void setPosX(int posX);
	void setPosY(int posY);
	
	Point move(int addX, int addY);
	
	void printPosition();
	
	Point getPosition() const;
    float getDistance(Point p) const;

};

#endif // POINT_H
