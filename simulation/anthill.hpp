#ifndef ANTHILL_H
#define ANTHILL_H

#include <vector>
#include "entity.hpp"
#include "ant.hpp"
#include <memory>

class World;
class Food;
class Anthill : public Entity
{
	/** all the food we have! hahahaha */
    std::vector<std::shared_ptr<Food>> food_;
public:
    Anthill(World& world, Point pos);
    ~Anthill();
	void step(int);

    void lift(std::shared_ptr<Food> &l);
    void lift(std::weak_ptr<Entity> &f);
};

#endif // ANTHILL_H
