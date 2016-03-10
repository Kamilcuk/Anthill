/*
 * Ant.h
 *
 *  Created on: 10 mar 2016
 *      Author: kamil
 */

#ifndef ANT_H_
#define ANT_H_

#include "Liftable.hpp"

class Ant {
	/** lifted by ant, or null */
	Liftable *liftable_;

	int speed_;
public:
	Ant();
	~Ant();

	void lift(Liftable& liftable);
	Liftable *unLift();
	Liftable *getLiftable() const;
};

#endif /* ANT_H_ */
