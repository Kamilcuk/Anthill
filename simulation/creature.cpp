#include "creature.hpp"
#include "world.hpp"
#include "ant.hpp"
#include <boost/make_shared.hpp>

void Creature::addLeg(){
    shared_ptr<Leg> p=boost::make_shared<Leg>(getWorld(), this);
    legs.push_back(p);
    getWorld().addUpdatable(p);
}
void Creature::addAntMandibles(){
    shared_ptr<AntMandibles> p=boost::make_shared<AntMandibles>(getWorld(), this);
    antMandibles.push_back(p);
    getWorld().addUpdatable(p);
}
void Creature::addAntSensor(){
    shared_ptr<AntSensor> p=boost::make_shared<AntSensor>(getWorld(), this);
    antSensors.push_back(p);
    getWorld().addUpdatable(p);
}
