#ifndef VISITABLE_HPP
#define VISITABLE_HPP

#include "visitor.hpp"

class World;

/**
 * Represents all objects that can be visited using visitor pattern.
 * Automatically adds pointer to self in World instance upon construction.
 */
class Visitable
{
    World* world_;
public:
    Visitable(World* world);
    virtual ~Visitable();
    
	Visitable& operator=(const Visitable&);
    
    virtual void accept(Visitor& v) const = 0;
};

#endif // VISITABLE_HPP
