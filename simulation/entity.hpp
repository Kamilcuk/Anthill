#ifndef LIVING_H
#define LIVING_H

#include "point.hpp"
#include "updatable.hpp"
#include <vector>
#include <memory>
#include <map>
#include <algorithm>
class World;
class Food;
class Anthill;


/**
 * @brief The Living class
 * All that lives shall be here!
 */
class Entity : public Updatable {

	/** position of this entity */
	Point pos_;

public:
	Entity(World &world, Point pos);
	virtual ~Entity();

    Point getPos() const;
    void setPos(Point pos);

    float getDistance(Entity * const e);

    /** physical informations **/
    virtual int getSmell(){ return 0;}
    virtual int getColorR(){return 0;}
    virtual int getColorG(){return 0;}
    virtual int getColorB(){return 0;}
    virtual int getColorA(){return 255;}

};

/* predeclarations */
class Food;
class Anthill;


#endif // LIVING_H
