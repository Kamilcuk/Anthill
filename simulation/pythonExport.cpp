/*
 * pythonExport.cpp
 *
 *  Created on: 10 mar 2016
 *      Author: ms
 */

#include "World.hpp"
#include "Ant.hpp"
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

using boost::shared_ptr;
BOOST_PYTHON_MODULE(anthill){
    using namespace boost::python;

    class_<World>("World",init<>())
        //.def("getAnts",&World::getAntsPythonList)
        .def("getAnts",&World::getAnts)
        .def("setDimensions",&World::setDimensions)
        .def("setSimulationFramerate",&World::setSimulationFramerate)
        .def("startSimulation",&World::startSimulation)
        .def("stopSimulation",&World::stopSimulation)
    ;

    class_<vector<int> >("vector_int")
        .def(vector_indexing_suite<vector<int> >() );


    class_<Ant>("Ant",init<>())
        //.def("getX",&Ant::getX)
    ;
}
