/*
 * World.cpp
 *
 *  Created on: 9 mar 2016
 *      Author: ms
 */

#include "world.hpp"
#include "anthill.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "pheromoneMap.hpp"
#include "ant.hpp"
#include "entity.hpp"
#include "food.hpp"
#include <algorithm>
#include "bodyParts.hpp"
#include "statistics.hpp"

using std::vector;

World::World()
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
void World::setSimulationFramerate(float){
}
void World::startSimulation()
{
	/* initialize random seed: */
	srand (time(NULL));

    /* statistics */
    visitors_.push_back(std::make_shared<Statistics>(*this));



	/* they add itself*/
	/* remember one pheromone map per world! */
    std::shared_ptr<PheromoneMap> to_food = std::make_shared<PheromoneMap>(*this, PheromoneMap::Type::ToFood, width, height, 0.01);
    addUpdatable(to_food);
    addUpdatable(std::make_shared<PheromoneMap>(*this, PheromoneMap::Type::FromFood, width, height, 0.01));



	ShapeGenerator shape_gen;

    addUpdatable(std::make_shared<Ant>(*this, Point(30,30)));
    addUpdatable(std::make_shared<Ant>(*this, Point(25,25)));
    addUpdatable(std::make_shared<Ant>(*this, Point(20,20)));
    addUpdatable(std::make_shared<Ant>(*this, Point(50,50)));
    addUpdatable(std::make_shared<Ant>(*this, Point(30,20)));

    addUpdatable(std::make_shared<Anthill>(*this, Point(35,35)));

	
	for(auto point : shape_gen.GenerateCircle(Point(15, 15), 3))
	{
        addUpdatable(std::make_shared<Food>(*this, point));
	}
	for(auto point : shape_gen.GenerateLine(Point(5, 40), Point(20, 20), 2))
	{
        addUpdatable(std::make_shared<Food>(*this, point));
	}

	for(auto point : shape_gen.GenerateLine(Point(10, 20), Point(30, 30), 3))
	{
		obstacles_.emplace_back(Obstacle(*this, point));
	}
}

void World::stopSimulation()
{
	updatables_.clear();
}

/* well, i know */
void World::simulationStep()
{
    std::cout<<"--------------------------------------------"<<std::endl;
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
}

void World::addUpdatable(std::shared_ptr<Updatable> e)
{
    auto it = std::find_if(updatables_.begin(), updatables_.end(),
                           [&](std::shared_ptr<Updatable> const &u){return u == e;});
    if ( it == updatables_.end() ) {
        // no such elemnt
        updatables_.push_back(e);
    }
}

void World::removeUpdatable(std::shared_ptr<Updatable> e)
{
    auto it = std::find_if(updatables_.begin(), updatables_.end(),
                           [&](std::shared_ptr<Updatable> const &u){return u == e;});
	if ( it == updatables_.end() ) {
        // no such elemnt
		return;
	}
	updatables_.erase(it);
}


std::vector<Obstacle*> World::getObstacles()
{
	std::vector<Obstacle*> ret;

    for(unsigned int i=0; i<obstacles_.size(); ++i)
        ret.push_back(&obstacles_[i]);
    return ret;
}

std::vector<std::shared_ptr<Updatable>> World::getUpdatables() const
{
    return updatables_;
}

std::vector<Updatable*> World::getPntUpdatables()
{
    std::vector<Updatable*> ret;
    //std::transform(updatables_.begin(), updatables_.end, ret.begin(), [](std::shared_ptr<Updatable> &u){return u.get();});
    for(auto& u : this->getUpdatables() ) {
        ret.push_back(u.get());
    }
    return ret;
}

template<typename Derived>
std::vector<std::shared_ptr<Derived>> World::getDerivedUpdatable()
{
    std::vector<std::shared_ptr<Derived>> ret;
    for( auto p : updatables_ ) {
        std::shared_ptr<Derived> d = std::dynamic_pointer_cast<Derived>(p);
        if ( d ) {
            ret.push_back(d);
        }
    }
    return ret;
}


template<typename Derived>
std::vector<Derived *> World::getPntDerivedUpdatable()
{
    std::vector<Derived *> ret;
    std::vector<std::shared_ptr<Derived>> in = getDerivedUpdatable<Derived>();
    ret.reserve(in.size());
    std::transform(in.begin(), in.end(), std::back_inserter(ret), [&](std::shared_ptr<Derived> &p){return p.get();});
    return ret;
}

#include "anthill.hpp"
#include "ant.hpp"
#include "pheromoneMap.hpp"
#include "food.hpp"
#include "entity.hpp"
template std::vector<Entity*> World::getPntDerivedUpdatable<Entity>();
template std::vector<Ant*> World::getPntDerivedUpdatable<Ant>();
template std::vector<Anthill*> World::getPntDerivedUpdatable<Anthill>();
template std::vector<PheromoneMap*> World::getPntDerivedUpdatable<PheromoneMap>();
template std::vector<Food*> World::getPntDerivedUpdatable<Food>();

template std::vector<std::shared_ptr<Entity>> World::getDerivedUpdatable<Entity>();
template std::vector<std::shared_ptr<Ant>> World::getDerivedUpdatable<Ant>();
template std::vector<std::shared_ptr<Anthill>> World::getDerivedUpdatable();
template std::vector<std::shared_ptr<PheromoneMap>> World::getDerivedUpdatable<PheromoneMap>();
template std::vector<std::shared_ptr<Food>> World::getDerivedUpdatable<Food>();
