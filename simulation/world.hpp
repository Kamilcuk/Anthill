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

#include "Ant.hpp"
#include "pythonExport.hpp"

class World{

    // world properties
    int width,height;
    float framerate;

    // objects in world
    vector<Ant> ants;


    void mainLoop();

public:
    World();
    ~World();
    boost::python::list getAntsPythonList();

    void setDimensions(int,int);
    void setSimulationFramerate(float);

    void startSimulation();
    void stopSimulation();
};

#endif /* WORLD_H_ */
