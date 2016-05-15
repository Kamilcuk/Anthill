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

Statistics::EntityCnt::EntityCnt() {
	existing = removed = init = 0;
}

void Statistics::EntityCnt::prepare() {

	existing = 0;
}

void Statistics::EntityCnt::count(const Entity &e, bool &firstCount) {
	if ( firstCount ) {
		init++;
	}
	if ( e.isFlaggedToRemove() ) {
		/* hope we never see this entity again. Never. We relay on hope. */
		removed++;
	} else {
		existing++;
	}
}

Statistics::EntityCnt Statistics::antCnt() const
{
	return antCnt_;
}

Statistics::EntityCnt Statistics::foodCnt() const
{
	return foodCnt_;
}

Statistics::EntityCnt Statistics::obstacleCnt() const
{
	return obstacleCnt_;
}

unsigned int Statistics::stepNumber() const
{
	return stepNumber_;
}

Statistics::Statistics()
{

}

const std::string print2(const Statistics::EntityCnt& u, const std::string app) {
	return std::string(
				app+" init count: "+std::to_string(u.init)+"\n"+
				app+" alive: "+std::to_string(u.existing)+"\n"+
				app+" dead: "+std::to_string(u.removed)+"\n"
			);
}

const std::string Statistics::print() const
{
	return std::string(
				print2(antCnt_, "ants")+
				print2(foodCnt_, "foods")+
				print2(obstacleCnt_, "obstacles")+
				"ants carring smth = " + std::to_string(antsCarring_) + "\n"+
				"step number = " + std::to_string(stepNumber_)+"\n"
			);
}

void Statistics::visit(const Ant &a)
{
	antCnt_.count(a, firstCount_);
}

void Statistics::visit(const Food &f)
{
	foodCnt_.count(f, firstCount_);
}

void Statistics::visit(const Obstacle &u)
{
	obstacleCnt_.count(u, firstCount_);
}

void Statistics::visit(const AntMandibles &u)
{
	if ( u.isHolding() ) {
		antsCarring_++;

		/* check if this is food? shlouldn't the ant know what and if its carrying a thing
		auto f = u.getHoldingObject();
		if ( f.lock() ) {
			if ( boost::dynamic_pointer_cast<Food>(f) ) {
				antsCarringFood_++;
			}
		}*/
	}
}

void Statistics::update(const std::vector<Visitable*>& vv)
{
	/* upate stepNumber_ */
	stepNumber_++;
	if ( firstCount_ ) {
		firstCount_ = ( stepNumber_ <= 1 );
	}

	/* prepare counters */
	antsCarring_ = 0;
	antCnt_.prepare();
	foodCnt_.prepare();
	obstacleCnt_.prepare();

	/* visit! */
	for (auto& v : vv) {
		v->accept(*this);
	}
}
