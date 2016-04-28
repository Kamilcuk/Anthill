#include "statistics.hpp"
#include "visitable.hpp"
#include <iostream>
#include "bodyParts.hpp"
#include "obstacle.hpp"
#include "world.hpp"
#include "food.hpp"
#include "ant.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

using namespace std;

Statistics::Statistics()
{
	this->reset();
}

const std::string Statistics::print() const
{
	return std::string( "Number ants alive: " + to_string(antCnt_.existing) + "\n" +
			"Number food alive: " + to_string(antCnt_.existing) + "\n" +
			"Init Food count: " + to_string(antCnt_.init) + "\n" +
			"init ant vound: " + to_string(antCnt_.init) + "\n" +
			"antsCarringFood_ = " + to_string(antsCarringFood_) + "\n" +
			"Numebr of obstacles: " + to_string(obstacleCnt_.existing) + "\n" +
			"initial number of obstacles: " + to_string(obstacleCnt_.init) + "\n" +
			"Number of ants that died: " + to_string(antCnt_.removed) + "\n");
}

void Statistics::visit(const Ant &a)
{
	count(antCnt_, a);
}

void Statistics::visit(const Food &f)
{
	count(foodCnt_, f);
}

void Statistics::visit(const AntMandibles &u)
{
	if ( u.isHolding() ) {
		antsCarringFood_++;

		/* check if this is food? shlouldn't the ant know what and if its carrying a thing
		auto f = u.getHoldingObject();
		if ( f.lock() ) {
			if ( boost::dynamic_pointer_cast<Food>(f) ) {
				antsCarringFood_++;
			}
		}*/
	}
}

void Statistics::visit(const Obstacle &u)
{
	count(obstacleCnt_, u);
}

void Statistics::reset()
{
	stepNumber_ = 0;
	antCnt_.reset();
	foodCnt_.reset();
	obstacleCnt_.reset();
}

void Statistics::update(const std::vector<Visitable*>& vv)
{
	stepNumber_++;
	antsCarringFood_ = 0;
	for (auto& v : vv) {
		v->accept(*this);
	}
}

void Statistics::count(UpdatableCount &c, const Updatable &u)
{
	if ( stepNumber_ == 1 ) {
		c.init++;
		c.existing++;
	}
	// if ( u.isFlaggedToRemove() ) {
	// 	c.existing--;
	// 	c.removed++;
	// }
}
