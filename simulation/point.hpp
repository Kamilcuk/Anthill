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
	constexpr Point() : posX_(0), posY_(0) {}
	constexpr Point(int posX, int posY) : posX_(posX), posY_(posY) {}
	constexpr Point(const Point &other) : posX_(other.posX_), 
		posY_(other.posY_) {}
	
    inline Point& operator=(const Point& rhs)
	{ posX_ = rhs.posX_; posY_ = rhs.posY_; return *this; }
	
	constexpr inline bool operator<(const Point& rhs) const // for std::set
	{ return (posX() < rhs.posX()) || (posY() < rhs.posY()); } 
	
	constexpr inline bool operator>(const Point& rhs) const // for std::set
	{ return (posX() > rhs.posX()) || (posY() > rhs.posY()); } 
	
	constexpr inline bool operator!=(const Point& rhs) const 
	{ return (posX() != rhs.posX()) || (posY() != rhs.posY()); } 
	
	constexpr inline bool operator==(const Point& rhs) const 
	{ return posX_ == rhs.posX_ && posY_ == rhs.posY_; }
	
	constexpr inline Point operator+(const Point& rhs) const 
	{ return Point(posX_ + rhs.posX_, posY_ + rhs.posY_); }
	
	constexpr inline Point operator-(const Point& rhs) const 
	{ return Point(posX_ - rhs.posX_, posY_ - rhs.posY_); }
	
	// dot product
	constexpr inline float operator*(const Point& rhs) 
	{ return posX_ * rhs.posX_ + posY_ * rhs.posY_; }
	
	constexpr inline Point operator*(const float& rhs) 
	{ return Point(posX_ * rhs, posY_ * rhs); }
	
	inline Point& operator+=(const Point& rhs) 
	{ posX_ += rhs.posX_; posY_ += rhs.posY_; return *this; }
	
	inline Point& operator-=(const Point& rhs) 
	{ posX_ -= rhs.posX_; posY_ -= rhs.posY_; return *this; }
	
	inline Point& operator*=(const float& rhs) 
	{ posX_ *= rhs; posY_ *= rhs; return *this; }
	
	std::string toString() const;
	
	constexpr inline int posX() const
	{ return posX_; }
	constexpr inline int posY() const
	{ return posY_; }
	
	void setPosX(int posX);
	void setPosY(int posY);
	
    constexpr inline float getDistance(Point p) const
	{
		return std::sqrt(
			std::pow(this->posX() - p.posX(), 2) +
			std::pow(this->posY() - p.posY(), 2)
			);
	}
	
    constexpr inline bool isInBounds(int size_x, int size_y) const
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
