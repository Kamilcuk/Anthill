#ifndef UPDATABLE_H
#define UPDATABLE_H


class World;
class Updatable {
	/** static? there's only one world!
	 * we could set that in World constructor? */
	World& world_;
public:
	Updatable(World& world);
	virtual ~Updatable();
	World& getWorld();
	virtual void step() = 0;
};

#endif // UPDATABLE_H
