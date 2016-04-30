/*
 * TODO: many files, 
 * but there would be too many of them 
 * if as many as classes
 */

#ifndef BODY_PARTS_H
#define BODY_PARTS_H

#include <iostream>
#include <vector>

#include <boost/weak_ptr.hpp>

#include "serialization.hpp"
#include "updatable.hpp"
#include "point.hpp"
#include "pheromoneMap.hpp"
#include "visitable.hpp"

class Entity;
class Creature;

class BodyPart : public Updatable{
protected:
    Creature* owner_;
public:
    BodyPart(World* w, Creature* c):
        Updatable(w), owner_(c){}
    virtual void step(int deltaTime){};
};

class AntLegs : public BodyPart{
    // how much time ant is standing in one place
    int timeNotMoving_;
    Point targetPos_;
public:
    AntLegs(World* w, Creature* owner);
    AntLegs(const AntLegs& o) :
        BodyPart(o) {}
    void goToPos(const Point& p);
    void goRandom();
    void step(int);
    int getTimeNotMoving(){ return timeNotMoving_; }
    
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & targetPos_;
	}
};

class AntMandibles;

class AntSensor : public BodyPart{
public:
    class Observation {
        boost::weak_ptr<Entity> ent_;
    public:
        Observation(boost::weak_ptr<Entity> e) : ent_(e)
        { }
        Point getPos()const;
        int getSmell()const;

        // All body parts implements whole physics.
        // AntMandibles must know more about this object
        // than external user (controller) and this user
        // can order AntMandibles to grab this Observation,
        // but cannot get internal Entity pointer
        // so,
        friend AntMandibles;
    };
    AntSensor(World* w, Creature* owner):
        BodyPart(w,owner){}

    std::vector<Observation> getEntities();
    
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
    {
	}
};

class AntMandibles : public BodyPart, virtual Visitable {
    boost::weak_ptr<Entity> holdingObject_;
public:
    AntMandibles(World* w, Creature* owner):
		Visitable(w),
		BodyPart(w,owner)
    {}
    bool grab(boost::weak_ptr<Entity> e);
    bool grab(AntSensor::Observation o);
    void step(int);
	bool isHolding() const { return !holdingObject_.expired(); }
    
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & holdingObject_;
	}

	// Visitable interface
public:
	void accept(Visitor &v) const;
	boost::weak_ptr<Entity> getHoldingObject() const;
};

class AntWorkerAbdomen : public BodyPart{
	PheromoneMap::Type dropType;
public:
    AntWorkerAbdomen(World* w, Creature* owner):
        BodyPart(w,owner),
        dropType(PheromoneMap::Type::None) // don't drop
    {};
    void dropToFoodPheromones();
    void dropFromFoodPheromones();
    void step(int);
    
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & dropType;
	}
};

class AntQueenAbdomen : public BodyPart{
	PheromoneMap::Type dropType;
public:
    AntQueenAbdomen(World* w, Creature* owner):
        BodyPart(w,owner),
        dropType(PheromoneMap::Type::None) // don't drop
    {};
    void dropAnthillPheromones();
    void step(int);
    
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & dropType;
	}
};


#endif // BODY_PARTS_H
