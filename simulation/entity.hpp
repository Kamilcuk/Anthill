#ifndef ENTITY_H
#define ENTITY_H

#include "updatable.hpp"
#include "point.hpp"

#include "serialization.hpp"

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
	Entity(World *world);
	virtual ~Entity();
    
	Entity(const Entity&);    
    Entity& operator=(const Entity&);

    /// We need to provide implementation so that Entity is serializable.
    virtual void step(int) {}

    Point getPos() const;
    void setPos(Point pos);

	/** return lenght of a straight line betweej this and the other entity */
    float getDistance(Entity * const e) const;

    /** physical informations **/
    virtual int getSmell(){ return 0;}
    virtual int getColorR(){return 0;}
    virtual int getColorG(){return 0;}
    virtual int getColorB(){return 0;}
    virtual int getColorA(){return 255;}
    
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & pos_;
	}
};

#endif // ENTITY_H
