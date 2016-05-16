#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_DYN_LINK
#endif

#include <boost/test/unit_test.hpp>

#include <boost/make_shared.hpp>

#include "../simulation/world.hpp"
#include "../simulation/updatable.hpp"
#include "../simulation/visitable.hpp"
#include "../simulation/entity.hpp"
#include "../simulation/creature.hpp"

namespace WorldTest
{
    
struct Fixture 
{
    World world;
    
    struct UpdatableConcrete : public Updatable
    {
        UpdatableConcrete(World* w) : Updatable(w) {}
        virtual void step(int) override {}
    };
    
    struct VisitableConcrete : public Visitable
    {
        VisitableConcrete(World* w) : Visitable(w) {}
        virtual void accept(Visitor& v) const override {}
    };
    
    struct ConcreteCreature : public Creature
    {
        ConcreteCreature(World* w) : Creature(w) {}
        virtual void step(int) override {}
    };
    
};

BOOST_FIXTURE_TEST_CASE(test_makeUpdatable_shouldAddToVectorInWorld, Fixture)
{
    BOOST_CHECK_EQUAL(world.getUpdatablePtrs().size(), 0);
    {
        auto test_instance = UpdatableConcrete(&world);
        BOOST_CHECK_EQUAL(world.getUpdatablePtrs().size(), 1);
        BOOST_CHECK_EQUAL(world.getUpdatablePtrs()[0], &test_instance);
    }
    BOOST_CHECK_EQUAL(world.getUpdatablePtrs().size(), 0);
}

BOOST_FIXTURE_TEST_CASE(test_makeVisitable_shouldAddToVectorInWorld, Fixture)
{   
    BOOST_CHECK_EQUAL(world.getVisitablePtrs().size(), 0);
    {
        auto test_instance = VisitableConcrete(&world);
        BOOST_CHECK_EQUAL(world.getVisitablePtrs().size(), 1);
        BOOST_CHECK_EQUAL(world.getVisitablePtrs()[0], &test_instance);
    }
    BOOST_CHECK_EQUAL(world.getVisitablePtrs().size(), 0);    
}

BOOST_FIXTURE_TEST_CASE(test_makeUntrackedEntity_shouldntBeStored, Fixture)
{
    BOOST_CHECK_EQUAL(world.getEntityPtrs().size(), 0);
    Creature entity_untracked(&world);
    BOOST_CHECK_EQUAL(world.getEntityPtrs().size(), 0);
}

BOOST_FIXTURE_TEST_CASE(test_makeTrackedEntity_shouldStore, Fixture)
{   
    BOOST_CHECK_EQUAL(world.getEntityPtrs().size(), 0);    
    world.addSimulationObject<Creature>(
        boost::make_shared<Creature>(&world));
    BOOST_CHECK_EQUAL(world.getEntityPtrs().size(), 1);
}

BOOST_FIXTURE_TEST_CASE(test_startSimulation_statisticsShouldWork, Fixture)
{
    BOOST_CHECK(world.getStatistics() == nullptr);
    world.startSimulation();
    BOOST_CHECK(world.getStatistics());
    BOOST_CHECK_EQUAL(world.getStatistics()->stepNumber(), 0);
    world.simulationStep();
    BOOST_CHECK_EQUAL(world.getStatistics()->stepNumber(), 1);
}

BOOST_FIXTURE_TEST_CASE(
    test_removeSimulationObject_shouldBeGoneImmediately, Fixture)
{   
    BOOST_CHECK_EQUAL(world.getSimulationObjects<Creature>().size(), 0);
    auto obj = world.addSimulationObject<Creature>(
        boost::make_shared<Creature>(&world));
        
    BOOST_CHECK_EQUAL(world.getSimulationObjects<Creature>().size(), 1);
    
    world.removeSimulationObject<Creature>(obj);
    
    BOOST_CHECK_EQUAL(world.getSimulationObjects<Creature>().size(), 0);
}

BOOST_FIXTURE_TEST_CASE(
    test_removeSimulationObject_shouldEntityGoneAtGetEntityPtrsCall, Fixture)
{   
    {
        BOOST_CHECK_EQUAL(world.getEntityPtrs().size(), 0);    
        auto obj = world.addSimulationObject<Creature>(
            boost::make_shared<Creature>(&world));
            
        BOOST_CHECK_EQUAL(world.getEntityPtrs().size(), 1);    
        
        world.removeSimulationObject<Creature>(obj);
        
        // there should still be one in getEntityPtrs(), because shared_ptr
        // to obj isn't invalidated yet, because there's still a local reference
        // to obj.
        BOOST_CHECK_EQUAL(world.getEntityPtrs().size(), 1);    
    } 
    // After local reference to obj gets destroyed, getEntityPtrs() should
    // remove expired pointers.
    BOOST_CHECK_EQUAL(world.getEntityPtrs().size(), 0);    
}

BOOST_FIXTURE_TEST_CASE(
    test_flagToRemoveSimulationObject_shouldBeGoneAfterStep, Fixture)
{   
    BOOST_CHECK_EQUAL(world.getSimulationObjects<Creature>().size(), 0);
    auto obj = world.addSimulationObject<Creature>(
        boost::make_shared<ConcreteCreature>(&world));
        
    BOOST_CHECK_EQUAL(world.getSimulationObjects<Creature>().size(), 1);  
    
    obj->flagToRemove();
    
    BOOST_CHECK_EQUAL(world.getSimulationObjects<Creature>().size(), 1);
    
    world.simulationStep();
    
    BOOST_CHECK_EQUAL(world.getSimulationObjects<Creature>().size(), 0);
}

} // namespace WorldTest