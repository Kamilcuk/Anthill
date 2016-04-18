/*
 * World.cpp
 *
 *  Created on: 9 mar 2016
 *      Author: ms
 */

#include "world.hpp"
#include "anthill.hpp"
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <cstdlib>
#include <ctime>
#include <boost/make_shared.hpp>
#include "pheromoneMap.hpp"
#include "ant.hpp"
#include "entity.hpp"
#include "food.hpp"
#include <algorithm>
#include "bodyParts.hpp"
#include "statistics.hpp"

using std::vector;


World::World() : statistics_(*this)
{
    setDimensions(300, 200);

    /* initialize random seed: */
	srand (time(NULL));
}

World::~World()
{
    this->stopSimulation();
}

void World::setDimensions(int X, int Y)
{
	width = X;
	height = Y;
}
void World::setSimulationFramerate(float)
{
}
void World::startSimulation()
{
	/* initialize random seed: */
	srand (time(NULL));

	/* they add itself*/
    /* remember one pheromone map per world! */
    addUpdatable(boost::make_shared<PheromoneMap>(*this, PheromoneMap::Type::ToFood, width, height, 0.01));
    addUpdatable(boost::make_shared<PheromoneMap>(*this, PheromoneMap::Type::FromFood, width, height, 0.01));

	ShapeGenerator shape_gen;

    addUpdatable(boost::make_shared<Ant>(*this, Point(30,30)));
    addUpdatable(boost::make_shared<Ant>(*this, Point(25,25)));
    addUpdatable(boost::make_shared<Ant>(*this, Point(20,20)));
    addUpdatable(boost::make_shared<Ant>(*this, Point(50,50)));
    addUpdatable(boost::make_shared<Ant>(*this, Point(30,20)));

    addUpdatable(boost::make_shared<Anthill>(*this, Point(35,35)));

	
	for(auto point : shape_gen.GenerateCircle(Point(15, 30), 3))
	{
        addUpdatable(boost::make_shared<Food>(*this, point));
	}
	for(auto point : shape_gen.GenerateLine(Point(5, 40), Point(20, 20), 2))
	{
        addUpdatable(boost::make_shared<Food>(*this, point));
	}

	//for(auto point : shape_gen.GenerateLine(Point(10, 20), Point(30, 30), 3))
	//{
	//	obstacles_.emplace_back(Obstacle(*this, point));
	//}
}

void World::stopSimulation()
{
	updatables_.clear();
}

/* well, i know */
void World::simulationStep()
{
    //std::cout<<"--------------------------------------------"<<std::endl;
    for(auto& u : updatables_) {
        u->step(1);
    }
    for(auto& u : updatables_) {
        u->step(0);
    }
    for(auto u = updatables_.begin(); u<updatables_.end(); ++u) {
        if ( !(*u)->isAlive() ){
            removeUpdatable(*u);
        }
    }
    statistics_.step(1);
}

void World::addUpdatable(boost::shared_ptr<Updatable> e)
{
    auto it = std::find_if(updatables_.begin(), updatables_.end(),
                           [&](boost::shared_ptr<Updatable> const &u){return u == e;});
    if ( it == updatables_.end() ) {
        // no such elemnt
        updatables_.push_back(e);
    }
}

void World::removeUpdatable(boost::shared_ptr<Updatable> e)
{
    auto it = std::find_if(updatables_.begin(), updatables_.end(),
                           [&](boost::shared_ptr<Updatable> const &u){return u == e;});
	if ( it == updatables_.end() ) {
        // no such elemnt
		return;
	}
	updatables_.erase(it);
}


std::vector<boost::shared_ptr<Obstacle>> World::getObstacles()
{
    return obstacles_;
}

std::vector<boost::shared_ptr<Updatable>> World::getUpdatables() const
{
    return updatables_;
}

template<typename Derived>
std::vector<boost::shared_ptr<Derived>> World::getDerivedUpdatable()
{
    std::vector<boost::shared_ptr<Derived>> ret;
    for( auto p : updatables_ ) {
        boost::shared_ptr<Derived> d = boost::dynamic_pointer_cast<Derived>(p);
        if ( d ) {
            ret.push_back(d);
        }
    }
    return ret;
}

Statistics World::getStatistics() const
{
    return statistics_;
}

#include "anthill.hpp"
#include "ant.hpp"
#include "pheromoneMap.hpp"
#include "food.hpp"
#include "entity.hpp"
#include "visitable.hpp"

template std::vector<boost::shared_ptr<Entity>> World::getDerivedUpdatable<Entity>();
template std::vector<boost::shared_ptr<Ant>> World::getDerivedUpdatable<Ant>();
template std::vector<boost::shared_ptr<Anthill>> World::getDerivedUpdatable();
template std::vector<boost::shared_ptr<PheromoneMap>> World::getDerivedUpdatable<PheromoneMap>();
template std::vector<boost::shared_ptr<Food>> World::getDerivedUpdatable<Food>();
template std::vector<boost::shared_ptr<Visitable>> World::getDerivedUpdatable<Visitable>();
