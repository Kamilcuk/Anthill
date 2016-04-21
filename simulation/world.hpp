/*
 * World.hpp
 *
 *  Created on: 9 mar 2016
 *      Author: ms
 */

#ifndef WORLD_H_
#define WORLD_H_

#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "shapeGenerator.hpp"
#include "obstacle.hpp"
#include "visitor.hpp"
#include "statistics.hpp"

class Ant;
class Updatable;
class Food;
class Entity;
class PheromoneMap;
class Anthill;
class Visitable;

/**
 * Represents simulation state and notifies all Updatable objetcts every 
 * simulation frame. Also, implements serialization/deserialization to allow
 * storage of simulation state in a file.
 */
class World {

    // world properties
    int width,height;
    float framerate;
    
    // For serialization of simulation state
    std::string filename_;
    
    // Pointers to stuff that is automatically updated in simualtion loop.
    // We're using an observer pattern here, so raw pointers are appropriate.
    // (http://stackoverflow.com/questions/17648912/)
    std::vector<Updatable*> updatable_ptrs_;
    
    // Pointers to stuff that shares some common properties for every physical
    // object in simulation. See @Entity for more details.
    // We're using an observer pattern here, so raw pointers are appropriate.    
    std::vector<Entity*> entity_ptrs_;
    
    // Pointers to stuff that is visitable.
    // We're using an observer pattern here, so raw pointers are appropriate. 
    std::vector<Visitable*> visitable_ptrs_;

    // Instances of simulation objects.
    std::vector<Food> foods_;
    std::vector<Obstacle> obstacles_;
    std::vector<Ant> ants_;
    std::vector<Anthill> anthills_;
    std::vector<PheromoneMap> pheromone_maps_;

    Statistics statistics_;
    
public:
    World();
    ~World();

	void setDimensions(int X, int Y);
    void setSimulationFramerate(float);

    /// Sets up simulation.
    void startSimulation();
    /// Clears all simulation state and stops simulation.
    void stopSimulation();
    /// Executed for every simulation step.
    void simulationStep();
    
    /// Serializes simulation state.
    void serializeState();
    /// Deserializes simulation state.
    void deserializeState();
    
    inline std::vector<Food>& getFoods() 
    { return foods_; }
    
    inline std::vector<Obstacle>& getObstacles() 
    { return obstacles_; }
    
    inline std::vector<Ant>& getAnts() 
    { return ants_; }
    
    inline std::vector<Anthill>& getAnthills() 
    { return anthills_; }
    
    inline std::vector<PheromoneMap>& getPheromoneMaps() 
    { return pheromone_maps_; }
    
    inline std::vector<Updatable*> & getUpdatablePtrs()
    { return updatable_ptrs_; }
    
    inline std::vector<Entity*> & getEntityPtrs()
    { return entity_ptrs_; }
    
    inline std::vector<Visitable*> & getVisitablePtrs()
    { return visitable_ptrs_; }

    Statistics getStatistics() const
    { return statistics_; }
    
private:
    friend class Updatable;
    friend class Entity;
    friend class Visitable;

    /// Adds updatable object to updatables_ptrs vector. Should be called only
    /// inside Updatable constructor!!
    void addUpdatable(Updatable* u);
    /// Removes an updatable from updatables_ptrs vector. Should be called only
    /// inside Updatable destructor!!
    void removeUpdatable(Updatable* u);
    
    /// Adds entity object to entities_ptrs vector. Should be called only
    /// inside Entity constructor!!
    void addEntity(Entity* e);
    /// Removes an entity from entities_ptrs vector. Should be called only
    /// inside Entity destructor!!
    void removeEntity(Entity* e);

    /// Adds visitable object to visitable_ptrs vector. Should be called only
    /// inside Visitable constructor!!
    void addVisitable(Visitable* v);
    /// Removes an Visitable from visitable_ptrs vector. Should be called only
    /// inside Visitable destructor!!
    void removeVisitable(Visitable* v);
};

#endif // WORLD_H_