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
    smell_=Smell::Ant;

    // Descriptions of Ant types (factory pattern implemented in constructor)
    if(type==Type::Worker){
        antLegs.emplace_back(boost::make_shared<AntLegs>(world, this));
        antMandibles.emplace_back(boost::make_shared<AntMandibles>(world, this));
        antSensors.emplace_back(boost::make_shared<AntSensor>(world, this));
        antWorkerAbdomens.emplace_back(boost::make_shared<AntWorkerAbdomen>(world,this));

        controller_=boost::make_shared<AntWorkerAI>(static_cast<Creature*>(this));

        maxEnergy_=200;
        energy_=maxEnergy_;

    }else if(type==Type::Queen){
        antLegs.emplace_back(boost::make_shared<AntLegs>(world, this));
        antMandibles.emplace_back(boost::make_shared<AntMandibles>(world, this));
        antSensors.emplace_back(boost::make_shared<AntSensor>(world, this));
        antQueenAbdomens.emplace_back(boost::make_shared<AntQueenAbdomen>(world,this));

        controller_=boost::make_shared<AntQueenAI>(static_cast<Creature*>(this));

        maxEnergy_=1000;
        energy_=maxEnergy_;

    }else if(type==Type::Scout){
        antLegs.emplace_back(boost::make_shared<AntLegs>(world, this));
        antMandibles.emplace_back(boost::make_shared<AntMandibles>(world, this));
        antSensors.emplace_back(boost::make_shared<AntSensor>(world, this));
        antWorkerAbdomens.emplace_back(boost::make_shared<AntWorkerAbdomen>(world,this));

        controller_=boost::make_shared<AntScoutAI>(static_cast<Creature*>(this));

        maxEnergy_=220;
        energy_=maxEnergy_;

    }else if(type==Type::Larva){
        hasCollision_=false;
        energy_=10;
        // no controller
    }
}

Ant::Ant(World* world) :
    Visitable(world),
    Creature(world)
{
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
        //std::cout<<"Don't have controller"<<std::endl;
    }

    energy_-=0.1;
    energy_=std::min(energy_,maxEnergy_);

    if(energy_<=0){
        flagToRemove();
    }
}

void Ant::accept(Visitor& v) const {
    v.visit(*this);
}

