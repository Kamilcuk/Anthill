#ifndef ANTHILL_H
#define ANTHILL_H

#include "entity.hpp"


class Anthill : public Entity
{
public:
	Anthill(Point pos);
	void step();
};

#endif // ANTHILL_H
