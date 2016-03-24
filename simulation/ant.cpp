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
    std::cout<<"Ant constructor"<<std::endl;
    addLeg();
    addAntMandibles();
    addAntSensor();
}

Ant::~Ant()
{
}

void Ant::step() {
    // test AI - all the below(like below) will be in Controller (similar context)
    // in this function will be probably only: controller.step()
    
    auto sensors=getAntSensors();
    auto mands=getAntMandibles();
    auto legs=getLegs();


    if(sensors.empty())
        std::cout<<"Nie mam sensorow\n";
    if(mands.empty())
        std::cout<<"Nie mam rzujek\n";
    if(legs.empty())
        std::cout<<"Nie mam nog\n";

    AntSensor& sensor=*sensors[0];
    AntMandibles& ma=*mands[0];
    Leg& leg=*legs[0];

    auto entities=sensor.getEntities();
    for(auto e : entities){
        // smell of food (enum ?)
        if((e->getSmell()==100) && !ma.isHolding()){
            if(e->getPos() != getPos())
                continue;
            ma.grab(e);
            break;
        }
    }

    leg.goToPos(Point(rand()%30+1,rand()%30+1));

}
