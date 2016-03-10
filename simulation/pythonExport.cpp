/*
 * pythonExport.cpp
 *
 *  Created on: 10 mar 2016
 *      Author: ms
 */

#include "world.hpp"
#include "ant.hpp"
#include <boost/python.hpp>

using boost::shared_ptr;
BOOST_PYTHON_MODULE(anthill){
    using namespace boost::python;

    class_<World>("World",init<>())
        .def("getAnts",&World::getAntsPythonList)
        .def("setDimensions",&World::setDimensions)
        .def("setSimulationFramerate",&World::setSimulationFramerate)
        .def("startSimulation",&World::startSimulation)
        .def("stopSimulation",&World::stopSimulation)
		.def("simulationStep",&World::simulationStep)
    ;

    //class_<Ant>("Ant",init<>())
        //.def("getX",&Ant::getX)
    //;
    // sorki! kamil
}
