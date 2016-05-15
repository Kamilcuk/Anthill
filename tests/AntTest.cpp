#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_DYN_LINK
#endif

#include <boost/test/unit_test.hpp>
#include <boost/make_shared.hpp>

#include "../simulation/ant.hpp"
#include "../simulation/world.hpp"

namespace AntTest
{

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