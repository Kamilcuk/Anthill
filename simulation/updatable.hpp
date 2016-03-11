#ifndef UPDATABLE_H
#define UPDATABLE_H

//#include "world.hpp"

class World;
class Updatable {
	World& world_;

public:
	Updatable(World& world);
	virtual ~Updatable();
	virtual void step() = 0;
};

#endif // UPDATABLE_H
