#ifndef LIVING_H
#define LIVING_H

#include "point.hpp"
#include "updatable.h"

/**
 * @brief The Living class
 * All that lives shall be here!
 */
class Entity : public Updatable {
	/** position of this entity */
	Point pos_;

	/** something can carry this entity*/
	Entity *carrier_;
public:
	Entity(Point pos);
	Point &getPos();

	void addCarrier(Entity *e);
	void removeCarrier();
	Entity *getCarrier() const;
};

#endif // LIVING_H
