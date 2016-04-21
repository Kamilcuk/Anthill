#ifndef ENTITY_H
#define ENTITY_H

#include "updatable.hpp"
#include "point.hpp"

class World;

/**
 * Represents all physical entities in simulation, like objects.
 * Upon creation, automatically adds pointer to this to generic entity_ptrs
 * list inside World instance provided in constructor. This allows entities to
 * get a list of all other entities easily and quickly.
 */
class Entity : public Updatable
{
	/** position of this entity */
	Point pos_;

public:
	Entity(World *world, Point pos);
	virtual ~Entity();

    Point getPos() const;
    void setPos(Point pos);

    float getDistance(Entity * const e)const;

    /** physical informations **/
    virtual int getSmell(){ return 0;}
    virtual int getColorR(){return 0;}
    virtual int getColorG(){return 0;}
    virtual int getColorB(){return 0;}
    virtual int getColorA(){return 255;}

};

#endif // ENTITY_H
