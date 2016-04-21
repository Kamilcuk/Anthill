/*
 * Ant.h
 *
 *  Created on: 10 mar 2016
 *      Author: kamil
 */

#ifndef ANT_H_
#define ANT_H_

#include "entity.hpp"
#include "world.hpp"
#include "creature.hpp"
#include "visitable.hpp"

class World;
class AntLegs;
class AntMandibles;
class AntSensor;
class AntWorkerAbdomen;

class Ant : public Creature, virtual Visitable {
	int speed_ = 1;
	int visibility_ = 4;
	
    // vectors of body parts
    std::vector<boost::shared_ptr<AntLegs> > antLegs;
    std::vector<boost::shared_ptr<AntMandibles> > antMandibles;
    std::vector<boost::shared_ptr<AntSensor> > antSensors;
    std::vector<boost::shared_ptr<AntWorkerAbdomen> > antWorkerAbdomens;
public:
	Ant(World* world, Point pos);
	virtual ~Ant();
	
	void step(int);
	
    // Visitable interface
    void accept(Visitor& v) const;
};

#endif /* ANT_H_ */
