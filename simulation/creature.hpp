#ifndef CREATURE_H
#define CREATURE_H

#include <vector>

#include "serialization.hpp"
#include "entity.hpp"
#include "bodyParts.hpp"

class Creature : public Entity
{
public:
    Creature(World* world, Point pos):
        Entity(world, pos) {}
    Creature(World* world):
        Entity(world) {}
        
    virtual void step(int) override
    {
        throw std::runtime_error("Creature::step called, should never happen.");
    }
        
private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Entity>(*this);
    }
};

#endif // CREATURE_H