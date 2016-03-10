#ifndef LIVING_H
#define LIVING_H

#include "point.hpp"
#include "updatable.h"

/**
 * @brief The Living class
 * All that lives shall be here!
 */
class Entity : public Updatable {
	Point pos_;
public:
	Entity(Point pos);
	Point getPos() const;
};

#endif // LIVING_H
