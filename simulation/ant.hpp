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
class AntQueenAbdomen;

class Ant : public Creature, virtual Visitable {

public:
	enum class Type 
	{
        Worker,
        Queen,
        Larva
	};

	Ant(World* world, Point pos,Type type=Type::Worker);
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
	}
};

#endif //ANT_H_
