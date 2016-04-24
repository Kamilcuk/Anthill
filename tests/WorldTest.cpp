#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_DYN_LINK
#endif

#include <boost/test/unit_test.hpp>

#include <boost/make_shared.hpp>

#include "../simulation/world.hpp"
#include "../simulation/updatable.hpp"
#include "../simulation/visitable.hpp"
#include "../simulation/entity.hpp"

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
    
    struct EntityConcrete : public Entity 
    {
        EntityConcrete(World* w) : Entity(w), some_value(0) {}
        int some_value;
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
    EntityConcrete entity_untracked(&world);
    BOOST_CHECK_EQUAL(world.getEntityPtrs().size(), 0);
}

BOOST_FIXTURE_TEST_CASE(
    test_makeTrackedEntityInLexicalScope_shouldBeImmediatelyExpired, Fixture)
{
    BOOST_CHECK_EQUAL(world.getEntityPtrs().size(), 0);
    world.trackEntity<EntityConcrete>(
        boost::make_shared<EntityConcrete>(&world));
    BOOST_CHECK_EQUAL(world.getEntityPtrs().size(), 0);
}

BOOST_FIXTURE_TEST_CASE(test_makeTrackedEntity_shouldStore, Fixture)
{   
    BOOST_CHECK_EQUAL(world.getEntityPtrs().size(), 0);    
    auto tracked_entity_moved = world.trackEntity<EntityConcrete>(
        boost::make_shared<EntityConcrete>(&world));
    BOOST_CHECK_EQUAL(world.getEntityPtrs().size(), 1);
    
    auto tracked_entity_copied = boost::make_shared<EntityConcrete>(&world);
    world.trackEntity<EntityConcrete>(tracked_entity_copied);
    BOOST_CHECK_EQUAL(world.getEntityPtrs().size(), 2);
}

BOOST_FIXTURE_TEST_CASE(
    test_makeTrackedEntityLocalScope_shouldExpireUponLeaving, Fixture)
{   
    BOOST_CHECK_EQUAL(world.getEntityPtrs().size(), 0);    
    {
        auto local_entity = world.trackEntity<EntityConcrete>(
            boost::make_shared<EntityConcrete>(&world));
        BOOST_CHECK_EQUAL(world.getEntityPtrs().size(), 1);
    }
    BOOST_CHECK_EQUAL(world.getEntityPtrs().size(), 0);
}

BOOST_FIXTURE_TEST_CASE(test_trackedEntitySetValue_shouldGetSameValue, Fixture)
{
    BOOST_CHECK_EQUAL(world.getEntityPtrs().size(), 0);

    const int test_val = 13;    
    
    auto tracked_entity = world.trackEntity<EntityConcrete>(
        boost::make_shared<EntityConcrete>(&world));
    tracked_entity->some_value = test_val;
        
    auto weak_entity = world.getEntityPtrs()[0];
    auto shared_concrete = boost::dynamic_pointer_cast<EntityConcrete>(
        weak_entity.lock());
    BOOST_CHECK_EQUAL(shared_concrete->some_value, tracked_entity->some_value);
}

} // namespace WorldTest