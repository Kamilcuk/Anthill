#ifndef CREATURE_H
#define CREATURE_H

#include <vector>

#include "serialization.hpp"
#include "entity.hpp"
#include "bodyParts.hpp"
#include "controller.hpp"

// for explanation of this line see serializationCustom.hpp
extern Creature* g_current_owner;

class Creature : public Entity
{
protected:
    boost::shared_ptr<Controller> controller_;
	
    // vectors of body parts
    std::vector<boost::shared_ptr<AntLegs> > antLegs;
    std::vector<boost::shared_ptr<AntMandibles> > antMandibles;
    std::vector<boost::shared_ptr<AntSensor> > antSensors;
    std::vector<boost::shared_ptr<AntWorkerAbdomen> > antWorkerAbdomens;
    std::vector<boost::shared_ptr<AntQueenAbdomen> > antQueenAbdomens;

public:

    std::vector<boost::shared_ptr<AntLegs> >& getAntLegs(){
        return antLegs;
    }
    std::vector<boost::shared_ptr<AntMandibles> >& getAntMandibles(){
        return antMandibles;
    }
    std::vector<boost::shared_ptr<AntSensor> >& getAntSensors(){
        return antSensors;
    }
    std::vector<boost::shared_ptr<AntWorkerAbdomen> >& getAntWorkerAbdomens(){
        return antWorkerAbdomens;
    }
    std::vector<boost::shared_ptr<AntQueenAbdomen> >& getAntQueenAbdomens(){
        return antQueenAbdomens;
    }

    Creature(World* world, Point pos):
        Entity(world, pos){
        hasCollision_=true;
    }
    Creature(World* world):
        Entity(world) {
        hasCollision_ = true;
    }
        
    virtual void step(int) override
    {
        throw std::runtime_error("Creature::step called, should never happen.");
    }

    float bite(float strength) override;
        
private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Entity>(*this);
        // for explanation of this line see serializationCustom.hpp
        g_current_owner = this; 
        ar & controller_;
		ar & antLegs;
        ar & antMandibles;
        ar & antSensors;
        ar & antWorkerAbdomens;
        ar & antQueenAbdomens;
    }

    // body parts can for example add or remove other body parts
    friend BodyPart;
    friend AntLegs;
    friend AntMandibles;
    friend AntWorkerAbdomen;
    friend AntQueenAbdomen;
};

#endif // CREATURE_H
