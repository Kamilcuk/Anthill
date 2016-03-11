#ifndef UPDATABLE_H
#define UPDATABLE_H


class World;
class Updatable {
	World& world_;
public:
	Updatable(World& world);
	virtual ~Updatable();
	World& getWorld();
	virtual void step() = 0;
};

#endif // UPDATABLE_H
