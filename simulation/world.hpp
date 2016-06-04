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
#include "visitor.hpp"
#include "statistics.hpp"

#include "entity2DMap.hpp"

#include "point.hpp"

class Food;
class Obstacle;
class Creature;
class Anthill;
class PheromoneMap;

class Entity;
class Updatable;
class Visitable;

/**
 * Represents simulation state and notifies all Updatable objects every
 * simulation frame. Also, implements serialization/deserialization to allow
 * storage of simulation state in a file.
 */
class World {
    friend class WorldGenerator;

    // world properties
    unsigned width_, height_;
    
    bool multithreaded_;
    
    // Pointers to stuff that is automatically updated in simualtion loop.
    // We're using an observer pattern here, so raw pointers are appropriate.
    // (http://stackoverflow.com/questions/17648912/)
    std::vector<Updatable*> updatable_ptrs_;
        
    // Pointers to stuff that is visitable.
    // We're using an observer pattern here, so raw pointers are appropriate. 
    std::vector<Visitable*> visitable_ptrs_;

    // Simulation objects.
    template<class T>
    using VectorOfSharedPtrs = std::vector<boost::shared_ptr<T> >;
    VectorOfSharedPtrs<Food> foods_;
    VectorOfSharedPtrs<Obstacle> obstacles_;
    VectorOfSharedPtrs<Creature> creatures_;
    VectorOfSharedPtrs<Anthill> anthills_;
    VectorOfSharedPtrs<PheromoneMap> pheromone_maps_;
    
    // Stores weak pointers to every simulation object, that has been tracked
    // using trackEntity() method. This is to ensure efficient retreival of
    // handles to all entities in the simulation.
    template<class T>
    using VectorOfWeakPtrs = std::vector<boost::weak_ptr<T> >;
    VectorOfWeakPtrs<Entity> entity_ptrs_;

    // 2D vector containing pointers to entities.
    boost::shared_ptr<Entity2DMap> entity_map_;

    // Stats object.
	boost::shared_ptr<Statistics> statistics_;
	bool statisticsEnabled_ = false;
    
    
public:
    World();
    ~World();

	void setDimensions(unsigned x, unsigned y);
	Point getDimensions();
    
    void setMultithreaded(bool m);
    bool isMultithreaded();

    /// Sets up simulation.
    void startSimulation();
    /// Clears (deletes) all simulation state and stops simulation.
    void stopSimulation();
    /// Executed for every simulation step.
    void simulationStep();
    
    
    /// Serializes simulation state.
    void saveState(std::string filename);
    /// Deserializes simulation state.
    void loadState(std::string filename);
    
    /// Returns reference to vector of specific simulation objects, for example
    /// of type Creature or Food. 
    template<class C>
    VectorOfSharedPtrs<C>& getSimulationObjects();
    
    /// Adds an object into a specific storage vector for specified type of 
    /// object (for example, adds to foods_ vector if specified type is Food).
    template<class C>
    boost::shared_ptr<C> addSimulationObject(boost::shared_ptr<C> obj)
    {
        trackEntity(boost::dynamic_pointer_cast<Entity>(obj));
        getSimulationObjects<C>().emplace_back(obj);
        return obj;
    }
    
    /// Removes an object from a specific storage vector for specified type of 
    /// object (for example, removes form foods_ vector if specified type is 
    /// Food).
    /// This shouldn't be called by simulation objects interacting with other 
    /// simulation objects! Use Entity::flagToRemove() instead.
    template<class C>
    void removeSimulationObject(boost::shared_ptr<C> obj)
    {
        auto& remove_from = getSimulationObjects<C>();
        remove_from.erase(
            std::remove(remove_from.begin(), remove_from.end(), obj),
            remove_from.end());
    }
    
    /// Removes simulation objects of specific type from their corresponding
    /// storage vector only and only if they were flagged to remove.
    /// This method should be called for every simulation object type after
    /// each simulation step is finished, so that the simulation state is
    /// updated.
    template<class C>
    void removeFlaggedSimulationObjects()
    {
        auto& remove_from = getSimulationObjects<C>();
        remove_from.erase(
            std::remove_if(remove_from.begin(), remove_from.end(), 
                [] (auto obj) 
                    { return obj->isFlaggedToRemove(); }),
            remove_from.end());
    }
    
    inline const std::vector<Updatable*>& getUpdatablePtrs() const
    { return updatable_ptrs_; }
    
    inline const std::vector<Visitable*>& getVisitablePtrs() const
    { return visitable_ptrs_; }
    
    inline boost::shared_ptr<Statistics> getStatistics() const
    { return statistics_; }
    
    inline boost::weak_ptr<Entity2DMap> getEntityMap()
    { return entity_map_; }
    
	bool getStatisticsEnabled() const;
	void setStatisticsEnabled(bool statisticsEnabled)
    { statisticsEnabled_=statisticsEnabled; }

private:
	// using friends here because we want following methods below to be called
    // in very specific situations.
    friend class Updatable;
    friend class Visitable;
    friend class Entity;

    /// Adds updatable object to updatables_ptrs vector. Should be called only
    /// inside Updatable constructor!!
    void addUpdatable(Updatable* u);
    
    /// Removes an updatable from updatables_ptrs vector. Should be called only
    /// inside Updatable destructor!!
    void removeUpdatable(Updatable* u);

    /// Updates all updatables with specified stepsize.
    void updateAll(int stepsize);

    /// Updates all updatables using specified stepsize in parallel.
    void updateAllInParallel(int stepsize);
    
    /// Updates all updatables using specified stepsize in serial.  
    void updateAllInSerial(int stepsize);

    /// Adds visitable object to visitable_ptrs vector. Should be called only
    /// inside Visitable constructor!!
    void addVisitable(Visitable* v);
    /// Removes an Visitable from visitable_ptrs vector. Should be called only
    /// inside Visitable destructor!!
    void removeVisitable(Visitable* v);
    
    /// Checks if specified shared_ptr is valid and adds a weak_ptr created from
    /// it to to entity_map_ vector.
    void trackEntity(boost::shared_ptr<Entity> e);
    

           
private:
    friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & width_;
		ar & height_;
	}
};

#endif // WORLD_H_
