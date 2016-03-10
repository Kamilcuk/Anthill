/*
 * Ant.cpp
 *
 *  Created on: 10 mar 2016
 *      Author: kamil
 */

#include "Ant.hpp"

Ant::Ant() :
	liftable_(nullptr),
	speed_(10) {

}

Ant::~Ant() {
}

void Ant::lift(Liftable& liftable) {
	liftable_ = &liftable;
}

Liftable *Ant::unLift() {
	if ( liftable_ != nullptr ) {
		Liftable *temp = liftable_;
		liftable_ = nullptr;
		return temp;
	}
	return nullptr;
}

Liftable *Ant::getLiftable() const {
	return liftable_;
}



