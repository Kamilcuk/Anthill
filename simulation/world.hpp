/*
 * World.hpp
 *
 *  Created on: 9 mar 2016
 *      Author: ms
 */

#ifndef WORLD_H_
#define WORLD_H_

#include <vector>
#include <utility>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "serialization.hpp"

#include "obstacle.hpp"
#include "visitor.hpp"
#include "statistics.hpp"

#include "ant.hpp"

class Ant;
class Updatable;
class Food;
class Entity;
class PheromoneMap;
class Anthill;
class Visitable;

/**
 * Represents simulation state and notifies all Updatable objects every
 * simulation frame. Also, implements serialization/deserialization to allow
 * storage of simulation state in a file.
 */
class World {
    friend class WorldGenerator;

    // world properties
    int width, height;
    float framerate;
    
    // Pointers to stuff that is automatically updated in simualtion loop.
    // We're using an observer pattern here, so raw pointers are appropriate.
    // (http://stackoverflow.com/questions/17648912/)
    std::vector<Updatable*> updatable_ptrs_;
        
    // Pointers to stuff that is visitable.
    // We're using an observer pattern here, so raw pointers are appropriate. 
    std::vector<Visitable*> visitable_ptrs_;

    // Simulation objects.
    std::vector<boost::shared_ptr<Food> > foods_;
    std::vector<boost::shared_ptr<Obstacle> > obstacles_;
    std::vector<boost::shared_ptr<Ant> > ants_;
    std::vector<boost::shared_ptr<Anthill> > anthills_;
    std::vector<boost::shared_ptr<PheromoneMap> > pheromone_maps_;
    
    // Stores pointers to every simulation object.
    std::vector<boost::weak_ptr<Entity> > entity_ptrs_;

    // Stats object.
    boost::shared_ptr<Statistics> statistics_;
    
public:
    World();
    ~World();

	void setDimensions(int x, int y);
    void setSimulationFramerate(float frames_per_sec);

    /// Sets up simulation.
    void startSimulation();
    /// Clears all simulation state and stops simulation.
    void stopSimulation();
    /// Executed for every simulation step.
    void simulationStep();
    
    /// Serializes simulation state.
    void saveState(std::string filename);
    /// Deserializes simulation state.
    void loadState(std::string filename);
        
    inline std::vector<boost::shared_ptr<Food> >& getFoods() 
    { return foods_; }
    
    inline std::vector<boost::shared_ptr<Obstacle> >& getObstacles() 
    { return obstacles_; }
    
    inline std::vector<boost::shared_ptr<Ant> >& getAnts() 
    { return ants_; }
    
    inline std::vector<boost::shared_ptr<Anthill> >& getAnthills() 
    { return anthills_; }
    
    inline std::vector<boost::shared_ptr<PheromoneMap> >& getPheromoneMaps() 
    { return pheromone_maps_; }
    
    inline std::vector<Updatable*>& getUpdatablePtrs()
    { return updatable_ptrs_; }
    
    inline std::vector<Visitable*>& getVisitablePtrs()
    { return visitable_ptrs_; }
    
    boost::shared_ptr<Statistics> getStatistics()
    { return statistics_; }
    
    /// Removes expired pointers and returns a vector of weak_ptrs of entities
    std::vector<boost::weak_ptr<Entity> >& getEntityPtrs();
    
private:
    // using friends here because we want methods below to be called in very
    // specific situations.
    friend class Updatable;
    friend class Visitable;
    friend class Entity;

    /// Adds updatable object to updatables_ptrs vector. Should be called only
    /// inside Updatable constructor!!
    void addUpdatable(Updatable* u);
    /// Removes an updatable from updatables_ptrs vector. Should be called only
    /// inside Updatable destructor!!
    void removeUpdatable(Updatable* u);

    /// Adds visitable object to visitable_ptrs vector. Should be called only
    /// inside Visitable constructor!!
    void addVisitable(Visitable* v);
    /// Removes an Visitable from visitable_ptrs vector. Should be called only
    /// inside Visitable destructor!!
    void removeVisitable(Visitable* v);
    
    /// Adds weak_ptr to specified Entity to entity_ptrs_ vector.
    void trackEntity(boost::shared_ptr<Entity> e);
    /// Invalidates list of entities, so that it is updated on the next call to
    /// getEntityPtrs(). Should be called in Entity destructor.
    void invalidateEntities();
    bool invalid_entities_ = true;
    
private:
    friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & width;
		ar & height;
	}
};

#endif // WORLD_H_
