#ifndef ANTHILL_H
#define ANTHILL_H

#include <vector>
#include <memory>

#include <boost/serialization/base_object.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "entity.hpp"
#include "ant.hpp"

class World;
class Food;
class Anthill : public Entity
{
    
	/** all the food we have! hahahaha */
    std::vector<boost::shared_ptr<Food>> food_;
public:
    Anthill(World* world, Point pos);
    Anthill(World* world);
    ~Anthill();
	void step(int);

    void lift(boost::shared_ptr<Food> &l);
    void lift(boost::weak_ptr<Entity> &f);
    
private:
    friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<Entity>(*this);
        ar & food_;
	}
};

#endif // ANTHILL_H
