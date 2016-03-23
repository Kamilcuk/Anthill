#ifndef UPDATABLE_H
#define UPDATABLE_H


class World;
class Updatable {
	/** static? there's only one world!
	 * we could set that in World constructor?
     * no, there is no need to make worldstatic,
     * it is'n using any special system resources
     * and we can create few worlds, why not?*/
protected:
	World& world_;
public:
	Updatable(World& world);
	virtual ~Updatable();
	World& getWorld();
	virtual void step() = 0;
};

#endif // UPDATABLE_H
