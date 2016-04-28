/*
 * Ant.h
 *
 *  Created on: 10 mar 2016
 *      Author: kamil
 */

#ifndef ANT_H_
#define ANT_H_

#include "serialization.hpp"

#include "entity.hpp"
#include "world.hpp"
#include "creature.hpp"
#include "visitable.hpp"
#include "controller.hpp"

class World;
class AntLegs;
class AntMandibles;
class AntSensor;
class AntWorkerAbdomen;

// for explanation of this line see serializationCustom.hpp
extern Creature* g_current_owner;

class Ant : public Creature, virtual Visitable {

public:
	Ant(World* world, Point pos);
	Ant(World* world);
	virtual ~Ant();
	
	void step(int);
	
    // Visitable interface
    void accept(Visitor& v) const;

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
        ar & boost::serialization::base_object<Creature>(*this);
        // for explanation of this line see serializationCustom.hpp
        g_current_owner = this; 
		ar & antLegs;
        ar & antMandibles;
        ar & antSensors;
        ar & antWorkerAbdomens;
	}
};

//class AntQueen : public Ant{
//public:
//    AntQueen(World*,Point);
//};

class AntWorker : public Ant, virtual Visitable{
public:
    AntWorker(World*,Point);
    AntWorker(World*);
};

#endif /* ANT_H_ */
