/*
 * pythonExport.cpp
 *
 *  Created on: 10 mar 2016
 *      Author: ms
 */

#include "world.hpp"
#include "entity.hpp"
#include "point.hpp"
#include "ant.hpp"
#include "food.hpp"
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

using boost::shared_ptr;
BOOST_PYTHON_MODULE(anthill){
    using namespace boost::python;

    class_<World>("World",init<>())
        .def("getAnts",&World::getAnts)
        .def("getFoods",&World::getFoods)
		.def("getAnthills",&World::getAnthills)
		.def("getObstacles",&World::getObstacles)
		.def("getPheromoneMaps",&World::getPheromoneMaps)
		.def("getUpdatables",&World::getUpdatables)

        .def("setDimensions",&World::setDimensions)
        .def("setSimulationFramerate",&World::setSimulationFramerate)
        .def("startSimulation",&World::startSimulation)
        .def("stopSimulation",&World::stopSimulation)
		.def("simulationStep",&World::simulationStep)

    ;

    class_<vector<Ant*> >("vector_ants")
        .def(vector_indexing_suite<vector<Ant*> >() );
    class_<vector<Food*> >("vector_food")
        .def(vector_indexing_suite<vector<Food*> >() );
    class_<vector<Anthill*> >("vector_anthills")
        .def(vector_indexing_suite<vector<Anthill*> >() );
    class_<vector<Obstacle*> >("vector_obstacles")
        .def(vector_indexing_suite<vector<Obstacle*> >() );
	class_<vector<PheromoneMap*> >("vector_pmaps")
		.def(vector_indexing_suite<vector<PheromoneMap*> >() );

    class_<vector<float> >("vector_float")
        .def(vector_indexing_suite<vector<float> >() );
    class_<vector<vector<float> > >("vector_vector_float")
        .def(vector_indexing_suite<vector< vector<float> > >() );


    class_<Ant,Ant*>("Ant",no_init)
		.def("getLoc",&Ant::getPos)
    ;
    class_<Food,Food*>("Food",no_init)
		.def("getLoc",&Food::getPos)
    ;
    class_<Anthill,Anthill*>("Anthill",no_init)
		.def("getLoc",&Anthill::getPos)
    ;
    class_<Obstacle,Obstacle*>("Obstacle",no_init)
		.def("getLoc",&Obstacle::getPos)
    ;

    class_<PheromoneMap,PheromoneMap*>("PheromoneMap",no_init)
		.def("getMapCopy",&PheromoneMap::getMapCopy)
    ;

    class_<Point,Point*>("Point",init<int,int>())
        .def("posX",&Point::posX)
        .def("posY",&Point::posY)
    ;

}
