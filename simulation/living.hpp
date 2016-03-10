#ifndef LIVING_H
#define LIVING_H

#include "position.hpp"

/**
 * @brief The Living class
 * All that lives shall be here!
 */
class Living : public Position{
public:
	Living(Position pos);
	virtual void step() = 0;
};

#endif // LIVING_H
