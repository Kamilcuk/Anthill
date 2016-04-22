#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "serialization.hpp"
#include "entity.hpp"

class Obstacle : public Entity
{
public:
	Obstacle(World* world, Point pos);
	Obstacle(World* world);
	void step(int);

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<Entity>(*this);
	}
};

#endif // OBSTACLE_H
