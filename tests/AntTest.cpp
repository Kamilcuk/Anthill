#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_DYN_LINK
#endif

#include <boost/test/unit_test.hpp>
#include <boost/make_shared.hpp>

#define private public
#define protected public

#include "../simulation/ant.hpp"
#include "../simulation/world.hpp"
#include "../simulation/food.hpp"

namespace AntTest
{

BOOST_AUTO_TEST_CASE( testAntLegs )
{
    World w;
    
    auto ant = w.addSimulationObject<Creature>(
        boost::make_shared<Ant>(&w,Point(10,10),Ant::Type::Worker));

    // assigning to private variable
    ant->controller_=boost::shared_ptr<Controller>();

    auto legs=ant->getAntLegs()[0];
    legs->goToPos(Point(30,40));

    for(int i=0;i<20;++i){
        Point lastPos=ant->getPos();
        w.simulationStep();
        Point newPos=ant->getPos();

        BOOST_CHECK_CLOSE(lastPos.getDistance(newPos),std::sqrt(2),0.0001);
        BOOST_CHECK(lastPos!=newPos);
        BOOST_CHECK(!lastPos.isAdjacent(newPos));
        BOOST_CHECK_EQUAL(newPos,lastPos+Point(1,1));
    }
    for(int i=0;i<10;++i){
        Point lastPos=ant->getPos();
        w.simulationStep();
        Point newPos=ant->getPos();

        BOOST_CHECK(lastPos.getDistance(newPos)==1.0);
        BOOST_CHECK(lastPos!=newPos);
        BOOST_CHECK(lastPos.isAdjacent(newPos));
        BOOST_CHECK_EQUAL(newPos,lastPos+Point(0,1));
    }
    for(int i=0;i<5;++i){
        Point lastPos=ant->getPos();
        w.simulationStep();
        Point newPos=ant->getPos();

        BOOST_CHECK(lastPos.getDistance(newPos)==0.0);
        BOOST_CHECK(lastPos==newPos);
        BOOST_CHECK(lastPos.isAdjacent(newPos));
        BOOST_CHECK_EQUAL(newPos,lastPos+Point(0,0));
    }
}

BOOST_AUTO_TEST_CASE( testAntDeath)
{
    World w;
    
    auto ant = w.addSimulationObject<Creature>(
        boost::make_shared<Ant>(&w,Point(10,10),Ant::Type::Worker));

    boost::weak_ptr<Creature> wp(ant);
    ant.reset();

    BOOST_CHECK(!wp.expired());
    for(int i=0;i<10;++i){
        w.simulationStep();
        // is alive after some steps, and no food
        BOOST_CHECK(!wp.expired());
    }    
    for(int i=0;i<100000;++i){
        w.simulationStep();
    }

    // is dead after many steps, and no food
    BOOST_CHECK(wp.expired());
}

BOOST_AUTO_TEST_CASE( testAntEating ){

    World w;
    
    auto p = w.addSimulationObject<Creature>(
        boost::make_shared<Ant>(&w,Point(10,10),Ant::Type::Worker));
    boost::weak_ptr<Creature> ant(p);
    p.reset();

    BOOST_CHECK(!ant.expired());

    // assigning to private variable
    ant.lock()->controller_=boost::shared_ptr<Controller>();

    auto ma=ant.lock()->getAntMandibles()[0];
    auto sensor=ant.lock()->getAntSensors()[0];

    for(int i=0;i<100;++i){
        BOOST_CHECK(!ant.expired());
        if(i%100==0)
            w.addSimulationObject<Food>(
                boost::make_shared<Food>(&w, Point(10,11)));

        for(auto o : sensor->getObservations()){
            if(o.getSmell() == Entity::Smell::Food){
                BOOST_CHECK_GT(ma->bite(o),0.0f);
                break;
            }
        }

        BOOST_CHECK_GT(ant.lock()->energy_,0.0f);
        BOOST_CHECK_LE(ant.lock()->energy_,ant.lock()->getMaxEnergy() + 10);

        w.simulationStep();
    }

    // is alive after many steps, because eats food 
    BOOST_CHECK(!ant.expired());
}


#undef private
#undef protected


BOOST_AUTO_TEST_CASE( testAntNoTeleportation )
{
    World w;
    
    auto p = w.addSimulationObject<Creature>(
        boost::make_shared<Ant>(&w));

    Point lastPos=p->getPos();
    for(int a=0;a<100;++a){
        Point pos=p->getPos();
        int dx=std::abs(pos.posX() - lastPos.posX());
        int dy=std::abs(pos.posY() - lastPos.posY());
        int dist=dx+dy;
        BOOST_CHECK_LE(dist,2);
        lastPos=pos;

        w.simulationStep();
    }
}

} // namespace AntTest
