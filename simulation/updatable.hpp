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
    
protected:
    World* world_;
};

#endif // UPDATABLE_H
