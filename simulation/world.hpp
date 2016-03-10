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

#include "ant.hpp"
#include "entity.hpp"
#include "food.hpp"
#include "pythonExport.hpp"
#include "updatable.h"

class Ant;
class World{

    // world properties
    int width,height;
    float framerate;

    // objects in world
    vector<Ant> ants;
    
	/** all living matter */
	std::vector<Updatable*> updatables_;


    void mainLoop();

public:
    World();
    ~World();
    boost::python::list getAntsPythonList();

    void setDimensions(int,int);
    void setSimulationFramerate(float);

    void startSimulation();
    void stopSimulation();
    
	/** executed for every simulation step */
	void simulationStep();

	void addLiving(Updatable* l);
	void removeLiving(Updatable* l);


	std::vector<Ant*> getAnts();
	std::vector<Food*> getFoods();
	std::vector<Updatable *> getClosestLivings(Point mypos, int visibility);

};

#endif /* WORLD_H_ */
