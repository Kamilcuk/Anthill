/*
 * World.cpp
 *
 *  Created on: 9 mar 2016
 *      Author: ms
 */

#include "World.hpp"
using std::vector;

World::World(){
}
World::~World(){
}

vector<int> World::getAntsPythonList(){
    return std_vector_to_py_list(ants);
}

boost::python::list World::getAntsPythonList(){
    return std_vector_to_py_list(ants);
}

void World::setDimensions(int,int){
}
void World::setSimulationFramerate(float){
}

void World::startSimulation(){
}
void World::stopSimulation(){
}
