#include "creature.hpp"
#include "world.hpp"
#include "ant.hpp"

void Creature::addAntLegs(){
    shared_ptr<AntLegs> p=std::make_shared<AntLegs>(world_, this);
    antLegs.push_back(p);
    world_.addUpdatable(p);
}
void Creature::addAntMandibles(){
    shared_ptr<AntMandibles> p=std::make_shared<AntMandibles>(world_, this);
    antMandibles.push_back(p);
    world_.addUpdatable(p);
}
void Creature::addAntSensor(){
    shared_ptr<AntSensor> p=std::make_shared<AntSensor>(world_, this);
    antSensors.push_back(p);
    world_.addUpdatable(p);
}
void Creature::addAntWorkerAbdomen(){
    shared_ptr<AntWorkerAbdomen> p=std::make_shared<AntWorkerAbdomen>(world_, this);
    antWorkerAbdomens.push_back(p);
    world_.addUpdatable(p);
}
