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


Ant::Ant(World& world, Point pos) :
	Creature(world, pos)
{
    addLeg();
    addAntMandibles();
    addAntSensor();
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
    auto legs=getLegs();


    if(sensors.empty()){
        std::cout<<"Nie mam sensorow\n";
        return;
    }
    if(mands.empty()){
        std::cout<<"Nie mam rzujek\n";
        return;
    }
    if(legs.empty()){
        std::cout<<"Nie mam nog\n";
        return;
    }

    AntSensor& sensor=*sensors[0];
    AntMandibles& ma=*mands[0];
    Leg& leg=*legs[0];

    auto observations=sensor.getEntities();
    for(auto e : observations){
        // smell of food (enum ?)
        if((e.getSmell()==100) && !ma.isHolding()){
            if(e.getPos() != getPos())
                continue;
            ma.grab(e);
            break;
        }
    }

    leg.goToPos(Point(rand()%30+1,rand()%30+1));

}
