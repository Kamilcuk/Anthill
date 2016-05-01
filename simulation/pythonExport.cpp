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
#include "creature.hpp"
#include "food.hpp"
#include "obstacle.hpp"
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
    REGISTER_SHAREDPTR(Creature);
    REGISTER_SHAREDPTR(Food);
    REGISTER_SHAREDPTR(Anthill);
    REGISTER_SHAREDPTR(Obstacle);
    REGISTER_SHAREDPTR(PheromoneMap);
    REGISTER_SHAREDPTR(Statistics);
    
#define REGISTER_VECTOR_SHAREDPTR(x) class_<vector<shared_ptr<x> > >(#x "_vec")\
        .def(vector_indexing_suite<vector<shared_ptr<x>>, true>() );
        
    REGISTER_VECTOR_SHAREDPTR(Creature);
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
    
// for specific template method:
#define REGISTER_WORLDGETOBJ_METHOD_REF(x) \
    .def("get"#x"s", &World::getSimulationObjects<x>,\
        return_internal_reference<>())
    
    REGISTER_CLASS(World)
        REGISTER_WORLDGETOBJ_METHOD_REF(Food)
        REGISTER_WORLDGETOBJ_METHOD_REF(Obstacle)
        REGISTER_WORLDGETOBJ_METHOD_REF(Creature)
        REGISTER_WORLDGETOBJ_METHOD_REF(Anthill)
        REGISTER_WORLDGETOBJ_METHOD_REF(PheromoneMap)
        REGISTER_METHOD(World, getStatistics)
        REGISTER_METHOD(World, setDimensions)
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
    
    REGISTER_CLASS_NOINIT(Creature)
        REGISTER_METHOD(Creature, getPos)
    ;
    REGISTER_CLASS_NOINIT(Food)
        REGISTER_METHOD(Food, getPos)
    ;
    REGISTER_CLASS_NOINIT(Anthill)
        REGISTER_METHOD(Anthill, getPos)
    ;   
    REGISTER_CLASS_NOINIT(Obstacle)
        REGISTER_METHOD(Obstacle, getPos)
    ;
    REGISTER_CLASS_NOINIT(PheromoneMap)
        REGISTER_METHOD_REF(PheromoneMap, getMap)
    ;
    REGISTER_CLASS_NOINIT(Point)
        REGISTER_METHOD(Point, posX)
        REGISTER_METHOD(Point, posY)
    ;    
    REGISTER_CLASS_NOINIT(Statistics)
        REGISTER_METHOD(Statistics, print)
    ;
}
