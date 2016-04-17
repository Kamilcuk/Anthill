/*
 * Ant.cpp
 *
 *  Created on: 10 mar 2016
 *      Author: kamil
 */

#include "ant.hpp"
#include <iostream>
#include <cstdlib>
#include "anthill.hpp"
#include "visitor.hpp"


Ant::Ant(World& world, Point pos) :
    Creature(world, pos)
{
    addAntLegs();
    addAntMandibles();
    addAntSensor();
    addAntWorkerAbdomen();
}

Ant::~Ant()
{
}

using std::shared_ptr;
using std::weak_ptr;

void Ant::step(int deltaTime) {
    if(!deltaTime)
        return;
    // test AI - all the below(like below) will be in Controller (similar context)
    // in this function will be probably only: controller.step()
    
    auto sensors=getAntSensors();
    auto mands=getAntMandibles();
    auto legsVec=getAntLegs();
    auto abdomensVec=getAntWorkerAbdomens();

    if(sensors.empty()){
        std::cout<<"Nie mam sensorow\n";
        return;
    }
    if(mands.empty()){
        std::cout<<"Nie mam rzujek\n";
        return;
    }
    if(legsVec.empty()){
        std::cout<<"Nie mam nog\n";
        return;
    }

    AntSensor& sensor=*sensors[0];
    AntMandibles& ma=*mands[0];
    AntLegs& legs=*legsVec[0];
    AntWorkerAbdomen& abd=*abdomensVec[0];

    bool targetPosChanged=0;
    Point targetPos=Point(rand()%40+1,rand()%40+1);

    auto observations=sensor.getEntities();
    for(auto o : observations){
        // smell of food (enum ?)
        if((o.getSmell()==100) && !ma.isHolding()){
            if(o.getPos() != getPos()){
                if(!targetPosChanged){
                    targetPos=o.getPos();
                    targetPosChanged=1;
                }
                continue;
            }
            ma.grab(o);
            std::cout<<"grabbed\n";
            break;
        }
    }

    if(!ma.isHolding()){
        abd.dropToFoodPheromones();
    }else{
        //TODO: return somewhere
        // example - point (0,0)
        legs.goToPos(Point(0,0));
        return;
    }

    legs.goToPos(targetPos);

}

void Ant::accept(Visitor& v) const {
    v.visit(*this);
}
