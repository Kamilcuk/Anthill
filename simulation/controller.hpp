#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "serialization.hpp"
#include "point.hpp"

class Creature;

class Controller{
protected:
    Creature* owner_;
public:
    Controller(Creature* owner){
        owner_=owner;
    }
    virtual void step(int deltaTime)=0;

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
    {
	}
};

class AntWorkerAI : public Controller{
    // ant starts to panic when is probably deadlocked with other ants
    int panicTimeLeft_;
    int timeSearchingWithFromFoodPheromones_;

public:
    AntWorkerAI(Creature* owner);
    void step(int deltaTime);

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Controller>(*this);
		ar & panicTimeLeft_;
        ar & timeSearchingWithFromFoodPheromones_;
	}
};

class AntQueenAI : public Controller{
public:
    AntQueenAI(Creature* owner);
    void step(int deltaTime);

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Controller>(*this);
	}
};

class AntScoutAI : public Controller{
    // ant starts to panic when is probably deadlocked with other ants
    int panicTimeLeft_;
    enum class Activity{
        ScanningArea,
        MarkingPathToFood
    };
    Activity currentActivity_;

public:
    AntScoutAI(Creature* owner);
    void step(int deltaTime);

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Controller>(*this);
		ar & panicTimeLeft_;
        ar & currentActivity_;
	}
};

#endif
