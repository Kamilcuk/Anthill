/*
 * Food.h
 *
 *  Created on: 10 mar 2016
 *      Author: kamil
 */

#ifndef SRC_FOOD_HPP_
#define SRC_FOOD_HPP_

#include "serialization.hpp"
#include "entity.hpp"

class Food : public Entity 
{
	bool used;
public:
	Food(World* world, Point pos);
	Food(World* world);

	void step(int);
	bool getUsed() const;
	void setUsed(bool value);

    int getSmell(){return 100;}

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<Entity>(*this);
		ar & used;
	}
};

#endif /* SRC_FOOD_HPP_ */
