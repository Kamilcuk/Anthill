#ifndef UPDATABLE_H
#define UPDATABLE_H

class World;

/**
 * Represents all objects that want to be updated in simulation loop.
 * If you want an object to be automatically added to notification list,
 * just inherit from this class and provide implementation of step().
 * Adding to correct notify lists is automatic during construction of 
 * Updatable(). In step() function define behaviour for different delta times.
 */
class Updatable
{
public:
    Updatable(World* world);
    Updatable(const Updatable&);
    virtual ~Updatable();
    
    // we need to provide following operators so that we can use references
    // to objects deriving from Updatable in Python module (Python lists use 
    // these operators)
	Updatable& operator=(const Updatable&);
    bool operator==(const Updatable&);
    bool operator!=(const Updatable&);
    
    /// Interface for single simulation step. @todo
	virtual void step(int delta_time) = 0;

    /// Since simulation is "double buffered", we don't want to be removed
    /// if other objects still want to interact with this. So if we want to be
    /// removed from simulation, we must be flagged to remove. After proper
    /// simulation step, this object will be removed from updatable notify list.
    inline void flagToRemove()
    {
        to_remove_flag_ = true;
    }
    
    /// @see flagToRemove
    inline bool isFlaggedToRemove() const
    {
        return to_remove_flag_;
    }
    
protected:
    World* world_;
    
private:
    bool to_remove_flag_ = false;  
};

#endif // UPDATABLE_H
