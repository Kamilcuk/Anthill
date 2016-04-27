#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "serialization.hpp"
#include "entity.hpp"
#include "visitable.hpp"

class Obstacle : public Entity, virtual Visitable
{
public:
	Obstacle(World* world, Point pos);
	Obstacle(World* world);

	/// Updatable interface
	void step(int delta_time);

	/// serialization
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<Entity>(*this);
	}

	// Visitable interface
public:
	void accept(Visitor &v) const;
};

#endif // OBSTACLE_H
