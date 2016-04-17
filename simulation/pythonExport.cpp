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
#include "anthill.hpp"
#include "pheromoneMap.hpp"
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <memory>
#include <boost/core/null_deleter.hpp>

using boost::shared_ptr;
using std::vector;

boost::shared_ptr<World> getSharedWorld() {
    return boost::shared_ptr<World>( &World::getInstance(), boost::null_deleter() );
}


//using boost::shared_ptr;
BOOST_PYTHON_MODULE(anthill){
    using namespace boost::python;

    boost::python::register_ptr_to_python<shared_ptr<World>>();
    boost::python::register_ptr_to_python<shared_ptr<Ant>>();
    boost::python::register_ptr_to_python<shared_ptr<Food>>();
    boost::python::register_ptr_to_python<shared_ptr<Anthill>>();
    boost::python::register_ptr_to_python<shared_ptr<Obstacle>>();
    boost::python::register_ptr_to_python<shared_ptr<PheromoneMap>>();

    class_<World, shared_ptr<World>, boost::noncopyable>("World", no_init)
        .def("getInstance",&getSharedWorld).staticmethod("getInstance")
        .def("getAnts",&World::getDerivedUpdatable<Ant>)
        .def("getFoods",&World::getDerivedUpdatable<Food>)
        .def("getAnthills",&World::getDerivedUpdatable<Anthill>)
        .def("getPheromoneMaps",&World::getDerivedUpdatable<PheromoneMap>)
        .def("getObstacles",&World::getObstacles)

        .def("setDimensions",&World::setDimensions)
        .def("setSimulationFramerate",&World::setSimulationFramerate)
        .def("startSimulation",&World::startSimulation)
        .def("stopSimulation",&World::stopSimulation)
		.def("simulationStep",&World::simulationStep)
    ;

    // remember , true> at the end!
    class_<vector<shared_ptr<Ant>> >("vector_ants")
        .def(vector_indexing_suite<vector<shared_ptr<Ant>>, true>() );
    class_<vector<shared_ptr<Food>> >("vector_food")
        .def(vector_indexing_suite<vector<shared_ptr<Food>>, true>() );
    class_<vector<shared_ptr<Anthill>> >("vector_anthills")
        .def(vector_indexing_suite<vector<shared_ptr<Anthill>>, true>() );
    class_<vector<shared_ptr<Obstacle>> >("vector_obstacles")
        .def(vector_indexing_suite<vector<shared_ptr<Obstacle>>, true>() );
    class_<vector<shared_ptr<PheromoneMap>> >("vector_pmaps")
        .def(vector_indexing_suite<vector<shared_ptr<PheromoneMap>>, true>() );

    class_<vector<float> >("vector_float")
        .def(vector_indexing_suite<vector<float> >() );
    class_<vector<vector<float> > >("vector_vector_float")
        .def(vector_indexing_suite<vector< vector<float> > >() );


    class_<Ant,shared_ptr<Ant>>("Ant",no_init)
        .def("getLoc",&Ant::getPos)
    ;
    class_<Food,shared_ptr<Food>>("Food",no_init)
        .def("getLoc",&Food::getPos)
    ;
    class_<Anthill,shared_ptr<Anthill>>("Anthill",no_init)
        .def("getLoc",&Anthill::getPos)
    ;
    class_<Obstacle,shared_ptr<Obstacle>>("Obstacle",no_init)
        .def("getLoc",&Obstacle::getPos)
    ;

    class_<PheromoneMap,shared_ptr<PheromoneMap>>("PheromoneMap",no_init)
        .def("getMapCopy",&PheromoneMap::getMapCopy)
    ;

    class_<Point>("Point",init<int,int>())
        .def("posX",&Point::posX)
        .def("posY",&Point::posY)
    ;

}
