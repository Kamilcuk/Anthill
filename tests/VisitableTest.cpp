#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_DYN_LINK
#endif

#include <boost/test/unit_test.hpp>

#include "../simulation/world.hpp"
#include "../simulation/visitable.hpp"
#include "../simulation/visitor.hpp"


namespace VisitableTest
{

struct Fixture 
{
    World world;
    
    struct ConcreteVisitable : public Visitable
    {
        ConcreteVisitable(World* w) : Visitable(w) {}
        virtual void accept(Visitor&) const override 
        {}
    };
    
    struct TestBaseVisitor : public Visitor
    {
        virtual void visit(const ConcreteVisitable& c) = 0;
    };
    
    struct TestConcreteVisitor : public TestBaseVisitor
    {
        bool visited = false;
        virtual void visit(const ConcreteVisitable& c) override
        {
            visited = true;
        }
    };
};

BOOST_FIXTURE_TEST_CASE(
    test_createVisitable_shouldBeAddedAndRemovedUponConstructionDestruction, 
    Fixture)
{
    BOOST_CHECK_EQUAL(world.getVisitablePtrs().size(), 0);
    {
        ConcreteVisitable u(&world);
        BOOST_CHECK_EQUAL(world.getVisitablePtrs().size(), 1);
    }
    BOOST_CHECK_EQUAL(world.getVisitablePtrs().size(), 0);
}

BOOST_FIXTURE_TEST_CASE(test_copyConstructor, Fixture)
{
    ConcreteVisitable v1(&world);
    ConcreteVisitable v2(v1);
    BOOST_CHECK_EQUAL(world.getVisitablePtrs().size(), 2);
}

BOOST_FIXTURE_TEST_CASE(test_visit, Fixture)
{
    TestConcreteVisitor visitor;
    ConcreteVisitable visitable(&world);
    visitor.visit(visitable);
    BOOST_CHECK(visitor.visited);
}

} // namespace VisitableTest