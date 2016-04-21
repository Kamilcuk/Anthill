#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "point.hpp"

#include "entity.hpp"
#include "obstacle.hpp"
#include "food.hpp"
#include "anthill.hpp"

/**
 * Boost::serialization uses default constructors of serialized objects by 
 * default, but our objects rarely have default constructors.
 * Therefore, we need to provide some additional function templates in order for 
 * serialization to work, namely: load_construct_data function templates.
 * See http://www.boost.org/doc/libs/1_53_0/libs/serialization/doc/serialization.html#constructors
 * for more details.
 */

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
 * Following are implementations of load_construct_data function temlates for 
 * every object that doesn't have a default constructor.
 */
 
/// helper macro for loading classess that have constructor(World*)
#define LOAD_PTR_WORLD(my_class) \
    template<class Archive> \
    inline void load_construct_data( \
        Archive & ar, my_class * t, const unsigned int file_version) \
    { ::new(t)my_class(g_world); } 

namespace boost
{
    namespace serialization
    {
        LOAD_PTR_WORLD(Entity);
        LOAD_PTR_WORLD(Obstacle);
        LOAD_PTR_WORLD(Food);
        LOAD_PTR_WORLD(Anthill);
    }
}


#endif // SERIALIZATION_H