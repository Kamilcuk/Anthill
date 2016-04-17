#pragma once
#include "entity.hpp"
#include "bodyParts.hpp"

#include <vector>

using boost::shared_ptr;

class Creature : public Entity{
    // vectors of body parts
    std::vector<shared_ptr<AntLegs> > antLegs;
    std::vector<shared_ptr<AntMandibles> > antMandibles;
    std::vector<shared_ptr<AntSensor> > antSensors;
    std::vector<shared_ptr<AntWorkerAbdomen> > antWorkerAbdomens;

protected:
    void addAntLegs();
    void addAntMandibles();
    void addAntSensor();
    void addAntWorkerAbdomen();
	Creature(World& world, Point pos):
        Entity(world,pos){}

public:
    std::vector<shared_ptr<AntLegs> >& getAntLegs(){
        return antLegs;
    }
    std::vector<shared_ptr<AntMandibles> >& getAntMandibles(){
        return antMandibles;
    }
    std::vector<shared_ptr<AntSensor> >& getAntSensors(){
        return antSensors;
    }
    std::vector<shared_ptr<AntWorkerAbdomen> >& getAntWorkerAbdomens(){
        return antWorkerAbdomens;
    }
};
