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
#include "visitable.hpp"

/**
 * Represents food.
 */
class Food : public Entity, virtual Visitable
{
	bool used;
public:
	Food(World* world, Point pos);
	Food(World* world);

	void step(int) override;
	bool getUsed() const;
	void setUsed(bool value);

    /// physics
    Entity::Smell getSmell(){return Entity::Smell::Food;}

	/// serialization
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<Entity>(*this);
		ar & used;
	}

	/// Visitable interface
public:
	void accept(Visitor &v) const;
};

#endif /* SRC_FOOD_HPP_ */
