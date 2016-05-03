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
#include "painter.hpp"
#include "entity.hpp"
#include "point.hpp"
#include "creature.hpp"
#include "food.hpp"
#include "obstacle.hpp"
#include "ant.hpp"
#include "anthill.hpp"
#include "pheromoneMap.hpp"
#include "statistics.hpp"

using boost::shared_ptr;
using std::vector;
using namespace boost::python;

// registers a shared_ptr
#define REGISTER_SHAREDPTR(x) boost::python::register_ptr_to_python\
    <shared_ptr<x>>();
    
// registers a vector of shared_ptr of objects of specified type
#define REGISTER_VECTOR_SHAREDPTR(x) class_<vector<shared_ptr<x> > >(#x "_vec")\
        .def(vector_indexing_suite<vector<shared_ptr<x>>, true>() );
        
// register a class to python
#define REGISTER_CLASS(cl) class_<cl>(#cl, init<>())

// registers a class that has no constructor
#define REGISTER_CLASS_NOINIT(cl) class_<cl>(#cl, no_init)

// registers a method with exported Python name same as specified name
#define REGISTER_METHOD(cl, meth) .def(#meth, &cl::meth)

// registers a method with custom exported Python name
#define REGISTER_METHOD_CUSTOM_NAME(cl, meth, name) .def(name, &cl::meth)

// same as REGISTER_METHOD but returns a reference
#define REGISTER_METHOD_REF(cl, meth) \
    .def(#meth, &cl::meth, return_internal_reference<>())

// registers a template method with specified name
#define REGISTER_TEMPLATE_METHOD(cl, meth, templ, name) \
    .def(name, &cl::meth<templ>)
    
// registers a template method that returns a reference. 
#define REGISTER_TEMPLATE_METHOD_REF(cl, meth, templ, name) \
    .def(name, &cl::meth<templ>, return_internal_reference<>())
    

BOOST_PYTHON_MODULE(anthill)
{
    REGISTER_SHAREDPTR(World);
    REGISTER_SHAREDPTR(WorldGenerator);
    REGISTER_SHAREDPTR(Creature);
    REGISTER_SHAREDPTR(Food);
    REGISTER_SHAREDPTR(Anthill);
    REGISTER_SHAREDPTR(Obstacle);
    REGISTER_SHAREDPTR(PheromoneMap);
    REGISTER_SHAREDPTR(Statistics);


    REGISTER_VECTOR_SHAREDPTR(Creature);
    REGISTER_VECTOR_SHAREDPTR(Food);
    REGISTER_VECTOR_SHAREDPTR(Anthill);
    REGISTER_VECTOR_SHAREDPTR(Obstacle);
    REGISTER_VECTOR_SHAREDPTR(PheromoneMap);

    class_<vector<float> >("vector_float")
        .def(vector_indexing_suite<vector<float> >() );
    class_<vector<vector<float> > >("vector_vector_float")
        .def(vector_indexing_suite<vector< vector<float> > >() );


    REGISTER_CLASS(World)
        REGISTER_TEMPLATE_METHOD_REF(World, getSimulationObjects, Food, "getFoods")
        REGISTER_TEMPLATE_METHOD_REF(World, getSimulationObjects, Obstacle, "getObstacles")
        REGISTER_TEMPLATE_METHOD_REF(World, getSimulationObjects, Creature, "getCreatures")
        REGISTER_TEMPLATE_METHOD_REF(World, getSimulationObjects, Anthill, "getAnthills")
        REGISTER_TEMPLATE_METHOD_REF(World, getSimulationObjects, PheromoneMap, "getPheromoneMaps")
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
        REGISTER_METHOD(WorldGenerator, initPheromoneMaps)
    ;
    REGISTER_CLASS(AnthillParams);
    REGISTER_CLASS(AntsParams);
    REGISTER_CLASS(ObstaclesParams);
    REGISTER_CLASS(FoodsParams);
    
    REGISTER_CLASS_NOINIT(Painter)
        REGISTER_TEMPLATE_METHOD(Painter, drawEntitiesWrapper, Food, "drawFoods")
        REGISTER_TEMPLATE_METHOD(Painter, drawEntitiesWrapper, Obstacle, "drawObstacles")
        REGISTER_METHOD_CUSTOM_NAME(Painter, drawPheromoneToFoodWrapper, "drawPheromoneToFood")
        REGISTER_METHOD_CUSTOM_NAME(Painter, drawPheromoneFromFoodWrapper, "drawPheromoneFromFood")
    ;    
    
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

#undef REGISTER_SHAREDPTR
#undef REGISTER_VECTOR_SHAREDPTR
#undef REGISTER_CLASS
#undef REGISTER_CLASS_NOINIT
#undef REGISTER_METHOD
#undef REGISTER_METHOD_REF
#undef REGISTER_TEMPLATE_METHOD
#undef REGISTER_TEMPLATE_METHOD_REF
    