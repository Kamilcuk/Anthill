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
#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include "entity.hpp"
#include "pythonExport.hpp"
#include "updatable.hpp"
#include "ant.hpp"
#include "food.hpp"

class Ant;
class Updatable;
class Food;
class Entity;
class World {

    // world properties
    int width,height;
    float framerate;
    
    /** all living matter */
	std::vector<Updatable *> updatables_;

    void mainLoop();

public:
    World();
    ~World();

    void setDimensions(int,int);
    void setSimulationFramerate(float);

    void startSimulation();
    void stopSimulation();
    
    /** executed for every simulation step */
    void simulationStep();

	void addUpdatable(Updatable* e);
	void removeUpdatable(Updatable* e);

    std::vector<Ant*> getAnts();
    std::vector<Food*> getFoods();
	std::vector<Entity *> getClosestEntities(Point mypos, int visibility);
	std::vector<Updatable *> getUpdatables() const;
};

#endif /* WORLD_H_ */
