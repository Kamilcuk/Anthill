#include "entity2DMap.hpp"

void Entity2DMap::applyAllMoves()
{
    for(auto move : moves_todo_)
        applyMove(move.what, move.from);
    moves_todo_.clear();
}

void Entity2DMap::removeExpired()
{
    if(!invalid_entities_)
        return;
    invalid_entities_ = false;
    for(unsigned x = 0; x < map_.size(); x++)
        for(unsigned y = 0; y < map_[0].size(); y++)
        {
            auto& cell = map_[x][y];
            cell.erase(std::remove_if(cell.begin(), cell.end(),
                    [] (auto e) { return e.expired(); }),
                cell.end());
        }
}