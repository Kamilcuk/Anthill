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
#include "worldGenerator.hpp"
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
#define REGISTER_SHAREDPTR(x) boost::python::register_ptr_to_python\
    <shared_ptr<x>>();
    REGISTER_SHAREDPTR(World);
    REGISTER_SHAREDPTR(WorldGenerator);
    REGISTER_SHAREDPTR(Ant);
    REGISTER_SHAREDPTR(Food);
    REGISTER_SHAREDPTR(Anthill);
    REGISTER_SHAREDPTR(Obstacle);
    REGISTER_SHAREDPTR(PheromoneMap);
    REGISTER_SHAREDPTR(Statistics);
    
#define REGISTER_VECTOR_SHAREDPTR(x) class_<vector<shared_ptr<x> > >(#x "_vec")\
        .def(vector_indexing_suite<vector<shared_ptr<x>>, true>() );
    REGISTER_VECTOR_SHAREDPTR(Ant);
    REGISTER_VECTOR_SHAREDPTR(Food);
    REGISTER_VECTOR_SHAREDPTR(Anthill);
    REGISTER_VECTOR_SHAREDPTR(Obstacle);
    REGISTER_VECTOR_SHAREDPTR(PheromoneMap);

    class_<vector<float> >("vector_float")
        .def(vector_indexing_suite<vector<float> >() );
    class_<vector<vector<float> > >("vector_vector_float")
        .def(vector_indexing_suite<vector< vector<float> > >() );
        
#define REGISTER_CLASS(cl) class_<cl>(#cl, init<>())
#define REGISTER_CLASS_NOINIT(cl) class_<cl>(#cl, no_init)

#define REGISTER_METHOD(cl, x) .def(#x, &cl::x)
#define REGISTER_METHOD_REF(cl, x) \
    .def(#x, &cl::x, return_internal_reference<>())
    
    REGISTER_CLASS(World)
        REGISTER_METHOD_REF(World, getAnts)
        REGISTER_METHOD_REF(World, getFoods)
        REGISTER_METHOD_REF(World, getAnthills)
        REGISTER_METHOD_REF(World, getPheromoneMaps)
        REGISTER_METHOD_REF(World, getObstacles)
        REGISTER_METHOD(World, getStatistics)
        REGISTER_METHOD(World, setDimensions)
        REGISTER_METHOD(World, setSimulationFramerate)
        REGISTER_METHOD(World, startSimulation)
        REGISTER_METHOD(World, stopSimulation)
        REGISTER_METHOD(World, simulationStep)
        REGISTER_METHOD(World, saveState)
        REGISTER_METHOD(World, loadState)
    ;
    
    REGISTER_CLASS_NOINIT(WorldGenerator)
        REGISTER_METHOD(WorldGenerator, placeAnthill)
        REGISTER_METHOD(WorldGenerator, placeAnts)
        REGISTER_METHOD(WorldGenerator, placeObstacles)
        REGISTER_METHOD(WorldGenerator, placeFoods)
    ;
    
    REGISTER_CLASS(AnthillParams);
    REGISTER_CLASS(AntsParams);
    REGISTER_CLASS(ObstaclesParams);
    REGISTER_CLASS(FoodsParams);
    
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
