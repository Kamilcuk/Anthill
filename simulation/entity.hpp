#ifndef LIVING_H
#define LIVING_H

#include "point.hpp"
#include "updatable.hpp"

/**
 * @brief The Living class
 * All that lives shall be here!
 */
class Entity : public Updatable {
	/** position of this entity */
	Point pos_;

	/** something can carry this entity*/
	Entity *carrier_;

	/** this entity can lift something */
	Entity *lifted_;

public:
	Entity(World &world, Point pos);
	virtual ~Entity();

	Point getPos();
	void setPos(Point pos);

	void addCarrier(Entity *e);
	Entity *removeCarrier();
	Entity *getCarrier() const;

	void lift(Entity *l);
	Entity *unLift();
	Entity *getLifted() const;

	float getDistance(Entity *e);

    /** physical informations **/
    virtual int getSmell(){ return 0;}
    virtual int getColorR(){return 0;}
    virtual int getColorG(){return 0;}
    virtual int getColorB(){return 0;}
    virtual int getColorA(){return 255;}
};

#endif // LIVING_H
