#ifndef POINT_H
#define POINT_H

#include "serialization.hpp"

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
	
    inline bool isInBounds(int size_x, int size_y)
    {
        return posX() >= 0 && posX() < size_x && posY() >= 0 && posY() < size_y;
    }
	
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & posX_;
		ar & posY_;
	}
};

#endif // POINT_H
