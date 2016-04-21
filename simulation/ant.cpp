/*
 * Ant.cpp
 *
 *  Created on: 10 mar 2016
 *      Author: kamil
 */

#include "ant.hpp"

#include <iostream>
#include <cstdlib>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "anthill.hpp"
#include "visitor.hpp"
#include "bodyParts.hpp"

Ant::Ant(World* world, Point pos) :
    Visitable(world),
    Creature(world, pos)
{
    antLegs.emplace_back(boost::make_shared<AntLegs>(world, this));
    antMandibles.emplace_back(boost::make_shared<AntMandibles>(world, this));
    antSensors.emplace_back(boost::make_shared<AntSensor>(world, this));
    antWorkerAbdomens.emplace_back(boost::make_shared<AntWorkerAbdomen>(world, 
        this));
}
    
Ant::~Ant()
{
}

using boost::shared_ptr;
using boost::weak_ptr;

void Ant::step(int deltaTime) {
    
    if(!deltaTime)
        return;
    // test AI - all the below(like below) will be in Controller (similar context)
    // in this function will be probably only: controller.step()
    
    if(antLegs.empty()){
        std::cout<<"Nie mam sensorow\n";
        return;
    }
    if(antMandibles.empty()){
        std::cout<<"Nie mam rzujek\n";
        return;
    }
    if(antSensors.empty()){
        std::cout<<"Nie mam nog\n";
        return;
    }
    if(antWorkerAbdomens.empty()){
        std::cout<<"Nie mam kaloryfera\n";
        return;
    }
    
    auto legs = antLegs[0];
    auto ma = antMandibles[0];
    auto sensor = antSensors[0];
    auto abd = antWorkerAbdomens[0];
    
    bool targetPosChanged=0;
    Point targetPos=Point(rand()%40+1,rand()%40+1);
        
    auto observations = sensor->getEntities();
    for(auto o : observations){
        // smell of food (enum ?)
        if((o.getSmell()==100) && !ma->isHolding()){
            if(o.getPos() != getPos()){
                if(!targetPosChanged){
                    targetPos=o.getPos();
                    targetPosChanged=1;
                }
                continue;
            }
            ma->grab(o);
            std::cout<<"grabbed\n";
            break;
        }
    }

    if(!ma->isHolding()){
        abd->dropToFoodPheromones();
    }else{
        //TODO: return somewhere
        // example - point (0,0)
        legs->goToPos(Point(0,0));
        return;
    }

    legs->goToPos(targetPos);
}

void Ant::accept(Visitor& v) const {
    v.visit(*this);
}
