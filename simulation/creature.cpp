#include "creature.hpp"
#include "world.hpp"
#include "ant.hpp"
#include <boost/make_shared.hpp>

void Creature::addLeg(){
    shared_ptr<Leg> p=boost::make_shared<Leg>(world_, this);
    legs.push_back(p);
    world_.addUpdatable(p);
}
void Creature::addAntMandibles(){
    shared_ptr<AntMandibles> p=boost::make_shared<AntMandibles>(world_, this);
    antMandibles.push_back(p);
    world_.addUpdatable(p);
}
void Creature::addAntSensor(){
    shared_ptr<AntSensor> p=boost::make_shared<AntSensor>(world_, this);
    antSensors.push_back(p);
    world_.addUpdatable(p);
}
