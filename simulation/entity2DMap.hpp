#ifndef ENTITY_2D_MAP_HPP
#define ENTITY_2D_MAP_HPP

#include <vector>

#include <boost/shared_ptr.hpp>

#include "entity.hpp"

/**
 * This class is a 2D container for vectors of weak pointers to Entities.
 * In other words: a 2D array; in each cell, any number of pointers to Entities
 * can be held.
 * Since these are weak pointers, they need to be "manually" cleared from the
 * map if they are expired. 
 * If an Entity changes its position, it MUST invoke move() method, so that its
 * position on the map is updated.
 * If an Entity is destroyed, it MUST invoke invalidate() method, so that the 
 * container knows that there exist expired pointers that need to be cleared.
 */
class Entity2DMap
{
    using WeakPtrEntity = boost::weak_ptr<Entity>;
    
    template<class T>
    using Map2D = std::vector<std::vector<T>>;
    
    
    Map2D<std::vector<WeakPtrEntity>> map_;
    
    /**
     * Helper struct that represents a single change of entitie's position.
     */
    struct ToDoMove
    {
        Point from;
        WeakPtrEntity what;  
        
        ToDoMove(WeakPtrEntity e, const Point& p) : 
            from(p), what(e)
        {}
    };
    
    std::vector<ToDoMove> moves_todo_;
    
    bool invalid_entities_;
        
public:
    Entity2DMap(unsigned size_x, unsigned size_y) : 
        map_(size_x, std::vector<std::vector<WeakPtrEntity>>(size_y)),
        invalid_entities_(false)
    {}

    /// Adds a new entity to the map.
    inline void add(WeakPtrEntity e)
    {
        auto pos = e.lock()->getPos();
        map_[pos.posX()][pos.posY()].emplace_back(e);
    }
    
    /// Call this function whenever an entity changes its position.
    /// This function will store the change, so that all changes can be
    /// applied at once using applyAllMoves().
    inline void move(WeakPtrEntity e, const Point& from)
    {
        moves_todo_.emplace_back(ToDoMove(e, from));
    }
    
    /// Returns all entities that are in specified square.
    inline std::vector<WeakPtrEntity> getEntitiesInSquare(Point corner_1,
        Point corner_2) const
    {
        std::vector<WeakPtrEntity> es;
        
        
        const auto size_x = map_.size(), size_y = map_[0].size();
        
        corner_1 = corner_1.clamped(Point(0, 0), Point(size_x, size_y));
        corner_2 = corner_2.clamped(Point(0, 0), Point(size_x, size_y));
        
        const int leftmost = std::min(corner_1.posX(), corner_2.posX()),
            rightmost = std::max(corner_1.posX(), corner_2.posX()),
            bottommost = std::min(corner_1.posY(), corner_2.posY()),
            topmost = std::max(corner_1.posY(), corner_2.posY());
            
        for(int x = leftmost; x < rightmost; x++)
            for(int y = bottommost; y < topmost; y++)
        {
            for(auto e : map_[x][y])
                es.emplace_back(e);
        }
        return es;
    }
    
    /// Returns all entities on the map.
    inline std::vector<WeakPtrEntity> getAllEntities() const
    {
        return getEntitiesInSquare(Point(0, 0), 
            Point(map_.size(), map_[0].size()));
    }
    
    /// This method should be called inside Entity destructor. It informs the
    /// class that some stored weak pointers are expired and need to be cleared.
    inline void invalidate()
    {
        invalid_entities_ = true;
    }
    
    /// Removes entity pointer from cell specified by "from" and adds it to 
    /// a correct cell.
    inline void applyMove(WeakPtrEntity e, const Point& from)
    {
        auto& cell = map_[from.posX()][from.posY()];
        cell.erase(std::remove_if(cell.begin(), cell.end(), 
                [e] (auto in_cell) { return e.lock() == in_cell.lock(); }), 
            cell.end());
        add(e); 
    }
    
    /// All moves stored using move() are applied using applyMove().
    void applyAllMoves();

    /// Traverses all cells and removes expired pointers.
    void removeExpired();

};

#endif // ENTITY_2D_MAP_HPP