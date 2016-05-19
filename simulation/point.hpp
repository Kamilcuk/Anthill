#ifndef POINT_H
#define POINT_H

#include <ostream>
#include <string>
#include <algorithm>

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
	
	constexpr inline Point clamped(const Point& p1, const Point p2)
	{	
		// for single coord: 
		// posX_ > leftmost ? (posX_ < rightmost ? posX_ : rightmost) : leftmost
		// Compiler bug: std::min doesn't qualify as constexpr
#define leftmost (p1.posX() > p2.posX() ? p2.posX() : p1.posX())
#define rightmost std::max(p1.posX(), p2.posX())
#define bottommost (p1.posY() > p2.posY() ? p2.posY() : p1.posY())
#define topmost std::max(p1.posY(), p2.posY())
;
		return Point(
			posX_ > leftmost ? (posX_ < rightmost ? posX_ : rightmost) 
				: leftmost,
			posY_ > bottommost ? (posY_ < topmost ? posY_ : topmost) 
				: bottommost);
#undef leftmost
#undef rightmost
#undef bottommost
#undef topmost			
	}
	
    constexpr inline float getDistance(Point p) const
	{
		return std::sqrt(
			std::pow(this->posX() - p.posX(), 2) +
			std::pow(this->posY() - p.posY(), 2)
			);
	}
	
    constexpr inline int getGridDistance(const Point& p) const
	{
		return
			std::abs(this->posX() - p.posX()) +
			std::abs(this->posY() - p.posY());
	}
	
    constexpr inline bool isInBounds(int size_x, int size_y) const
    {
        return posX() >= 0 && posX() < size_x && posY() >= 0 && posY() < size_y;
    }
	
    constexpr inline bool isInBounds(const Point& p) const
    {
        return isInBounds(p.posX(), p.posY());
    }
	
    constexpr inline bool isInBounds(const Point& p1, const Point& p2) const
    {
		// WTF?!?!: getting error: call to non-constexpr function std::min
		// if first line of return statement utilizes (like the other 3 lines)
		// the std::min function. 
		return posX() >= (p1.posX() < p2.posX() ? p1.posX() : p2.posX()) && 
			posX() <= std::max(p1.posX(), p2.posX()) && 
			posY() >= std::min(p1.posY(), p2.posY()) &&
			posY() <= std::max(p1.posY(), p2.posY());
    }

    constexpr inline bool isAdjacent(const Point& p) const
	{
		return std::abs(posX() - p.posX()) + std::abs(posY() - p.posY()) <= 1;
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
