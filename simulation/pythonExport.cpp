/*
 * pythonExport.cpp
 *
 *  Created on: 10 mar 2016
 *      Author: ms
 */

#include <vector>

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include "world.hpp"
#include "entity.hpp"
#include "point.hpp"
#include "ant.hpp"
#include "food.hpp"
#include "anthill.hpp"
#include "pheromoneMap.hpp"
#include "statistics.hpp"

using boost::shared_ptr;
using std::vector;
using namespace boost::python;

BOOST_PYTHON_MODULE(anthill)
{
/*
    boost::python::register_ptr_to_python<shared_ptr<World>>();
    boost::python::register_ptr_to_python<shared_ptr<Ant>>();
    boost::python::register_ptr_to_python<shared_ptr<Food>>();
    boost::python::register_ptr_to_python<shared_ptr<Anthill>>();
    boost::python::register_ptr_to_python<shared_ptr<Obstacle>>();
    boost::python::register_ptr_to_python<shared_ptr<PheromoneMap>>();
*/

    class_<vector<Ant> >("vector_ants")
        .def(vector_indexing_suite<vector<Ant>>() );
    class_<vector<Food> >("vector_food")
        .def(vector_indexing_suite<vector<Food>>() );
    class_<vector<Anthill> >("vector_anthills")
        .def(vector_indexing_suite<vector<Anthill>>() );
    class_<vector<Obstacle> >("vector_obstacles")
        .def(vector_indexing_suite<vector<Obstacle>>() );
    class_<vector<PheromoneMap> >("vector_pmaps")
        .def(vector_indexing_suite<vector<PheromoneMap>>() );

    class_<vector<float> >("vector_float")
        .def(vector_indexing_suite<vector<float> >() );
    class_<vector<vector<float> > >("vector_vector_float")
        .def(vector_indexing_suite<vector< vector<float> > >() );

#define WORLD_METHOD(x) .def(#x, &World::x)
#define WORLD_METHOD_REF(x) .def(#x, &World::x, return_internal_reference<>())
    class_<World>("World", init<>())
        WORLD_METHOD_REF(getAnts)
        WORLD_METHOD_REF(getFoods)
        WORLD_METHOD_REF(getAnthills)
        WORLD_METHOD_REF(getPheromoneMaps)
        WORLD_METHOD_REF(getObstacles)
        WORLD_METHOD(getStatistics)
        WORLD_METHOD(setDimensions)
        WORLD_METHOD(setSimulationFramerate)
        WORLD_METHOD(startSimulation)
        WORLD_METHOD(stopSimulation)
        WORLD_METHOD(simulationStep)
    ;
    
    class_<Ant>("Ant",no_init)
        .def("getLoc",&Ant::getPos)
    ;
    class_<Food>("Food",no_init)
        .def("getLoc",&Food::getPos)
    ;
    class_<Anthill>("Anthill",no_init)
        .def("getLoc",&Anthill::getPos)
    ;
    class_<Obstacle>("Obstacle",no_init)
        .def("getLoc",&Obstacle::getPos)
    ;
    class_<PheromoneMap>("PheromoneMap",no_init)
        .def("getMapCopy",&PheromoneMap::getMapCopy)
    ;
    class_<Point>("Point",init<int,int>())
        .def("posX",&Point::posX)
        .def("posY",&Point::posY)
    ;
    class_<Statistics>("Statistics",no_init)
            .def("print",&Statistics::print)
    ;
}
