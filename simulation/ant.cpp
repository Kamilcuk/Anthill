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
    controller_=boost::shared_ptr<Controller>(new AntWorkerAI(static_cast<Creature*>(this)));
}

Ant::Ant(World* world) :
    Visitable(world),
    Creature(world)
{
    antLegs.emplace_back(boost::make_shared<AntLegs>(world, this));
    antMandibles.emplace_back(boost::make_shared<AntMandibles>(world, this));
    antSensors.emplace_back(boost::make_shared<AntSensor>(world, this));
    antWorkerAbdomens.emplace_back(boost::make_shared<AntWorkerAbdomen>(world, 
        this));
    controller_=boost::shared_ptr<Controller>(new AntWorkerAI(static_cast<Creature*>(this)));
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
    }
}

void Ant::accept(Visitor& v) const {
    v.visit(*this);
}


// AntQueen
//AntQueen::AntQueen(World* world, Point pos):
//    Ant(world,pos)
//{
//    // TODO: antQueenAbdomen, AntQueenController
//}

//AntWorker::AntWorker(World* world, Point pos):
//    Visitable(world),
//    Ant(world,pos)
//{
//    antWorkerAbdomens.emplace_back(boost::make_shared<AntWorkerAbdomen>(world, 
//        this));
//    controller_=shared_ptr<Controller>(new AntWorkerAI(static_cast<Creature*>(this)));
//}
//AntWorker::AntWorker(World* world):
//    Visitable(world),
//    Ant(world)
//{
//    antWorkerAbdomens.emplace_back(boost::make_shared<AntWorkerAbdomen>(world, 
//        this));
//    controller_=shared_ptr<Controller>(new AntWorkerAI(static_cast<Creature*>(this)));
//}
