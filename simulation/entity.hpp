#ifndef ENTITY_H
#define ENTITY_H

#include <stdexcept>

#include <boost/enable_shared_from_this.hpp>

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
class Entity : public Updatable, public boost::enable_shared_from_this<Entity>
{
	/** position of this entity */
	Point pos_;
    
public:
    enum class Smell {
        Food,
        Ant,
        None
    };

protected:
    // for different derived classes different meaning,
    // but all of them need this variable
    // ex. Ant life energy, Obstacle endurance, Food energy, etc.
    bool hasCollision_;
    float energy_;
    float maxEnergy_;
    Smell smell_;

public:

	Entity(World *world, Point pos);
	Entity(World *world);
	virtual ~Entity();
    
	Entity(const Entity&);    
    Entity& operator=(const Entity&);

    inline bool hasCollision() const
    {
        return hasCollision_;
    }

    /// We need to provide implementation so that Entity is serializable.
    inline virtual void step(int) override
    {
        throw std::runtime_error("Entity::step called, should never happen.");
    }
    
    /// Since simulation is "double buffered", we don't want to be removed
    /// if other objects still want to interact with this. So if we want to be
    /// removed from simulation, we must be flagged to remove. After proper
    /// simulation step, this object will be removed from updatable notify list.
    inline void flagToRemove()
    {
        to_remove_flag_ = true;
    }
    
    /// @see flagToRemove()
    inline bool isFlaggedToRemove() const
    {
        return to_remove_flag_;
    }
    

    Point getPos() const;
    void setPos(Point pos);

	/** return lenght of a straight line betweej this and the other entity */
    float getDistance(Entity * const e) const;


    // physics
    virtual Smell getSmell() const { return smell_; }
    virtual int getColorR() const { return 0; }
    virtual int getColorG() const { return 0; }
    virtual int getColorB() const { return 0; }
    virtual int getColorA() const { return 255; }

    // have yourself bitten,
    // returns energy that can be eaten
    virtual float bite(float strength) { return 0; }

    float getEnergy() const { return energy_; }
    float getMaxEnergy() const { return maxEnergy_; }
    
private:
    bool to_remove_flag_ = false;  
    
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & pos_;
        ar & hasCollision_;
        ar & energy_;
        ar & maxEnergy_;
        ar & smell_;
        ar & to_remove_flag_;
	}

};

#endif // ENTITY_H
