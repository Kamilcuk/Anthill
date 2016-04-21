#include "creature.hpp"

#include "world.hpp"
#include "ant.hpp"

void Creature::addAntLegs(){
    antLegs.emplace_back(AntLegs(world_, this));
}
void Creature::addAntMandibles(){
    antMandibles.emplace_back(AntMandibles(world_, this));
}
void Creature::addAntSensor(){
    antSensors.emplace_back(AntSensor(world_, this));
}
void Creature::addAntWorkerAbdomen(){
    antWorkerAbdomens.emplace_back(AntWorkerAbdomen(world_, this));
}
