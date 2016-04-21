#pragma once

#include <vector>

#include "entity.hpp"
#include "bodyParts.hpp"

class Creature : public Entity{
    // vectors of body parts
    std::vector<AntLegs> antLegs;
    std::vector<AntMandibles> antMandibles;
    std::vector<AntSensor> antSensors;
    std::vector<AntWorkerAbdomen> antWorkerAbdomens;

protected:
    void addAntLegs();
    void addAntMandibles();
    void addAntSensor();
    void addAntWorkerAbdomen();
	Creature(World* world, Point pos):
        Entity(world, pos) {}

public:

    inline std::vector<AntLegs>& getAntLegs(){
        return antLegs;
    }
    inline std::vector<AntMandibles>& getAntMandibles(){
        return antMandibles;
    }
    inline std::vector<AntSensor>& getAntSensors(){
        return antSensors;
    }
    inline std::vector<AntWorkerAbdomen>& getAntWorkerAbdomens(){
        return antWorkerAbdomens;
    }
};
