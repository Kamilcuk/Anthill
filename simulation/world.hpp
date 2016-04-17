/*
 * World.hpp
 *
 *  Created on: 9 mar 2016
 *      Author: ms
 */

#ifndef WORLD_H_
#define WORLD_H_

#include <vector>
using std::vector;

#include "pythonExport.hpp"
#include "shapeGenerator.hpp"
#include "obstacle.hpp"
#include "visitor.hpp"

class Ant;
class Updatable;
class Food;
class Entity;
class PheromoneMap;
class Anthill;

/**
 * @brief The World class
 * World -> object factory
 */
class World {

    // world properties
    int width,height;
    float framerate;
    
    /** all living matter */
    std::vector<std::shared_ptr<Updatable>> updatables_;
    /** visitors */
    std::vector<std::shared_ptr<Visitor>> visitors_;

    std::vector<Obstacle> obstacles_;

public:
    World();
    ~World();

	void setDimensions(int X, int Y);
    void setSimulationFramerate(float);

    void startSimulation();
    void stopSimulation();
    
    /** executed for every simulation step */
    void simulationStep();

    void addUpdatable(std::shared_ptr<Updatable> e);
    void removeUpdatable(std::shared_ptr<Updatable> e);

    std::vector<std::shared_ptr<Updatable>> getUpdatables() const;
    std::vector<Updatable *> getPntUpdatables();
    std::vector<Obstacle *> getObstacles();

public:
    template<typename Derived>
        std::vector<std::shared_ptr<Derived>> getDerivedUpdatable();

    template<typename Derived>
       std::vector<Derived*> getPntDerivedUpdatable();
};

/** extern instantations -> declared in world.cpp*/
extern template std::vector<Entity*> World::getPntDerivedUpdatable<Entity>();
extern template std::vector<Ant*> World::getPntDerivedUpdatable<Ant>();
extern template std::vector<Anthill*> World::getPntDerivedUpdatable<Anthill>();
extern template std::vector<PheromoneMap*> World::getPntDerivedUpdatable<PheromoneMap>();
extern template std::vector<Food*> World::getPntDerivedUpdatable<Food>();

extern template std::vector<std::shared_ptr<Entity>> World::getDerivedUpdatable<Entity>();
extern template std::vector<std::shared_ptr<Ant>> World::getDerivedUpdatable<Ant>();
extern template std::vector<std::shared_ptr<Anthill>> World::getDerivedUpdatable();
extern template std::vector<std::shared_ptr<PheromoneMap>> World::getDerivedUpdatable<PheromoneMap>();
extern template std::vector<std::shared_ptr<Food>> World::getDerivedUpdatable<Food>();


#endif /* WORLD_H_ */
