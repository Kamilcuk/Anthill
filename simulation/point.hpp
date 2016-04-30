#ifndef POINT_H
#define POINT_H

#include <ostream>
#include <string>

#include "serialization.hpp"

class Point
{
	int posX_;
	int posY_;
	friend std::ostream& operator<<(std::ostream &os, const Point& rhs);
public:
    Point():posX_(0),posY_(0){}
	Point(int posX, int posY);
	Point(const Point& pos);
	
    inline Point& operator=(const Point& rhs)
	{ posX_ = rhs.posX_; posY_ = rhs.posY_; return *this; }
	
	inline bool operator<(const Point& rhs) const // for std::set
	{ return (posX() < rhs.posX()) || (posY() < rhs.posY()); } 
	
	inline bool operator>(const Point& rhs) const // for std::set
	{ return (posX() > rhs.posX()) || (posY() > rhs.posY()); } 
	
	inline bool operator!=(const Point& rhs) const 
	{ return (posX() != rhs.posX()) || (posY() != rhs.posY()); } 
	
	inline bool operator==(const Point& rhs) const 
	{ return posX_ == rhs.posX_ && posY_ == rhs.posY_; }
	
	inline Point operator+(const Point& rhs) const 
	{ return Point(posX_ + rhs.posX_, posY_ + rhs.posY_); }
	
	inline Point operator-(const Point& rhs) const 
	{ return Point(posX_ - rhs.posX_, posY_ - rhs.posY_); }
	
	// dot product
	inline float operator*(const Point& rhs) 
	{ return posX_ * rhs.posX_ + posY_ * rhs.posY_; }
	
	inline Point operator*(const float& rhs) 
	{ return Point(posX_ * rhs, posY_ * rhs); }
	
	inline Point& operator+=(const Point& rhs) 
	{ posX_ += rhs.posX_; posY_ += rhs.posY_; return *this; }
	
	inline Point& operator-=(const Point& rhs) 
	{ posX_ -= rhs.posX_; posY_ -= rhs.posY_; return *this; }
	
	inline Point& operator*=(const float& rhs) 
	{ posX_ *= rhs; posY_ *= rhs; return *this; }
	
	std::string toString() const;
	
	int posX() const;
	int posY() const;
	
	void setPosX(int posX);
	void setPosY(int posY);
	
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

extern std::ostream& operator<<(std::ostream &os, const Point& rhs);


#endif // POINT_H
