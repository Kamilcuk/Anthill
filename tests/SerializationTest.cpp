#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_DYN_LINK
#endif

#include <string>
#include <cstdio> // remove 

#include <boost/test/unit_test.hpp>

#include <boost/make_shared.hpp>

#include "../simulation/world.hpp"
#include "../simulation/anthill.hpp"
#include "../simulation/pheromoneMap.hpp"
#include "../simulation/ant.hpp"
#include "../simulation/food.hpp"
#include "../simulation/obstacle.hpp"

namespace SerializationTest
{
    
BOOST_AUTO_TEST_CASE(test_saveAndLoadState_shouldHaveSameNumOfEntities)
{
    // assumption here is that boost::serialization is properly tested
    // and works correctly.
    // Also, each serialized class should implement own serialization tests
    // comparing saved to loaded object.
    
    World to_save;

    BOOST_CHECK_EQUAL(to_save.getEntityPtrs().size(), 0);
    
    to_save.addSimulationObject<Anthill>(
        boost::make_shared<Anthill>(&to_save));
    to_save.addSimulationObject<PheromoneMap>(
        boost::make_shared<PheromoneMap>(&to_save));
    to_save.addSimulationObject<Creature>(
        boost::make_shared<Ant>(&to_save));
    to_save.addSimulationObject<Food>(
        boost::make_shared<Food>(&to_save));
    to_save.addSimulationObject<Obstacle>(
        boost::make_shared<Obstacle>(&to_save));

    BOOST_CHECK_EQUAL(to_save.getEntityPtrs().size(), 4);
    
    
    std::string filename = "test_world_serialization";
    to_save.saveState(filename);

    World to_load;
    BOOST_CHECK_EQUAL(to_load.getEntityPtrs().size(), 0);
    to_load.loadState(filename);
    BOOST_CHECK_EQUAL(to_load.getEntityPtrs().size(), 4);
    
    remove(filename.c_str());
}

} // namespace SerializationTest