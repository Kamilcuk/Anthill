/*
 * World.hpp
 *
 *  Created on: 9 mar 2016
 *      Author: ms
 */

#ifndef WORLD_H_
#define WORLD_H_

#include <vector>
#include "shapeGenerator.hpp"
#include "obstacle.hpp"
#include "visitor.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "statistics.hpp"

class Ant;
class Updatable;
class Food;
class Entity;
class PheromoneMap;
class Anthill;
class Visitable;

/**
 * @brief The World class
 * World -> object factory
 * singleton
 */
class World {

    // world properties
    int width,height;
    float framerate;
    
    /** all living matter */
    std::vector<boost::shared_ptr<Updatable>> updatables_;

    std::vector<boost::shared_ptr<Obstacle>> obstacles_;

    Statistics statistics_;
public:
    World();
    ~World();

	void setDimensions(int X, int Y);
    void setSimulationFramerate(float);

    void startSimulation();
    void stopSimulation();
    
    /** executed for every simulation step */
    void simulationStep();

    void addUpdatable(boost::shared_ptr<Updatable> e);
    void removeUpdatable(boost::shared_ptr<Updatable> e);

    std::vector<boost::shared_ptr<Updatable>> getUpdatables() const;
    std::vector<boost::shared_ptr<Obstacle>> getObstacles();

public:
    template<typename Derived>
        std::vector<boost::shared_ptr<Derived>> getDerivedUpdatable();
public:
    Statistics getStatistics() const;
};

extern template std::vector<boost::shared_ptr<Entity>> World::getDerivedUpdatable<Entity>();
extern template std::vector<boost::shared_ptr<Ant>> World::getDerivedUpdatable<Ant>();
extern template std::vector<boost::shared_ptr<Anthill>> World::getDerivedUpdatable();
extern template std::vector<boost::shared_ptr<PheromoneMap>> World::getDerivedUpdatable<PheromoneMap>();
extern template std::vector<boost::shared_ptr<Food>> World::getDerivedUpdatable<Food>();
extern template std::vector<boost::shared_ptr<Visitable>> World::getDerivedUpdatable<Visitable>();


#endif /* WORLD_H_ */
