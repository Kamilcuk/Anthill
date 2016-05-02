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


Ant::Ant(World* world, Point pos,Type type) :
	Visitable(world),
	Creature(world, pos)
{
    antLegs.emplace_back(boost::make_shared<AntLegs>(world, this));
    antMandibles.emplace_back(boost::make_shared<AntMandibles>(world, this));
    antSensors.emplace_back(boost::make_shared<AntSensor>(world, this));

    if(type==Type::Worker){
        antWorkerAbdomens.emplace_back(boost::make_shared<AntWorkerAbdomen>(world,this));
        controller_=boost::shared_ptr<Controller>(new AntWorkerAI(static_cast<Creature*>(this)));
    }else if(type==Type::Queen){
        antQueenAbdomens.emplace_back(boost::make_shared<AntQueenAbdomen>(world,this));
        controller_=boost::shared_ptr<Controller>(new AntQueenAI(static_cast<Creature*>(this)));
    }
}

Ant::Ant(World* world) :
    Visitable(world),
    Creature(world)
{
    //antLegs.emplace_back(boost::make_shared<AntLegs>(world, this));
    //antMandibles.emplace_back(boost::make_shared<AntMandibles>(world, this));
    //antSensors.emplace_back(boost::make_shared<AntSensor>(world, this));
    //antWorkerAbdomens.emplace_back(boost::make_shared<AntWorkerAbdomen>(world, this));
    //controller_=boost::shared_ptr<Controller>(new AntWorkerAI(static_cast<Creature*>(this)));
}

Ant::~Ant()
{
}

using boost::shared_ptr;
using boost::weak_ptr;

void Ant::step(int deltaTime) {
    
    if(!deltaTime)
        return;

    if(controller_ != nullptr){
        controller_->step(deltaTime);
    }else{
        std::cout<<"Don't have controller"<<std::endl;
    }
}

void Ant::accept(Visitor& v) const {
    v.visit(*this);
}

