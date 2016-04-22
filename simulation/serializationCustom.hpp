#ifndef SERIALIZATION_CUSTOM_H
#define SERIALIZATION_CUSTOM_H
/**
 * Boost::serialization uses default constructors of serialized objects by 
 * default, but our objects rarely have default constructors.
 * Therefore, we need to provide some additional function templates in order for 
 * serialization to work, namely: load_construct_data function templates.
 * See http://www.boost.org/doc/libs/1_53_0/libs/serialization/doc/serialization.html#constructors
 * for more details.
 */

#include "serialization.hpp"

#include "point.hpp"
#include "entity.hpp"
#include "obstacle.hpp"
#include "food.hpp"
#include "anthill.hpp"
#include "ant.hpp"
#include "creature.hpp"
#include "pheromoneMap.hpp"

/**
 * Many objects require pointer to World object in their constructors.
 * There is no simple way to pass it as an argument to load_construct_data
 * functions, so we will do it via a global variable.
 * This variable must be properly set before serialization can take place,
 * otherwise I dunno what could happen.
 */
class World;
World* g_world;

/**
 * Body parts require to know their owner upon creation. This global variable
 * must be changed for every deserialized Creature, which might prove 
 * problematic.
 */
Creature* g_current_owner;

/**
 * Following are implementations of load_construct_data function temlates for 
 * every object that doesn't have a default constructor.
 */
 
/// helper macro for loading classess that have constructor(World*)
#define LOAD_PTR_WORLD(my_class) \
    template<class Archive> \
    inline void load_construct_data( \
        Archive & ar, my_class * t, const unsigned int file_version) \
    { ::new(t)my_class(g_world); } 
    
/// helper macro for loading classess that have constructor(World*,Creature*)
#define LOAD_PTR_WORLD_AND_OWNER(my_class) \
    template<class Archive> \
    inline void load_construct_data( \
        Archive & ar, my_class * t, const unsigned int file_version) \
    { ::new(t)my_class(g_world, g_current_owner); } 

namespace boost
{
    namespace serialization
    {
        LOAD_PTR_WORLD(Entity);
        LOAD_PTR_WORLD(Obstacle);
        LOAD_PTR_WORLD(Food);
        LOAD_PTR_WORLD(Anthill);
        LOAD_PTR_WORLD(Ant);
        LOAD_PTR_WORLD(Creature);
        LOAD_PTR_WORLD(PheromoneMap);
        
        LOAD_PTR_WORLD_AND_OWNER(AntLegs);
        LOAD_PTR_WORLD_AND_OWNER(AntMandibles);
        LOAD_PTR_WORLD_AND_OWNER(AntSensor);
        LOAD_PTR_WORLD_AND_OWNER(AntWorkerAbdomen);
        
        // Followiong is serialization of PheromoneMap::Type enum class
        template<class Archive>
        void serialize(Archive & ar, PheromoneMap::Type& t, const unsigned int)
        {
            ar & make_binary_object(&t, sizeof(t));
        }
    }
}


#endif // SERIALIZATION_CUSTOM_H