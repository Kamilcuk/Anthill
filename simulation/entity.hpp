#ifndef LIVING_H
#define LIVING_H

#include "point.hpp"
#include "updatable.hpp"
#include <vector>
#include <memory>
#include <map>
#include <algorithm>
class World;
class Food;
class Anthill;


/**
 * @brief The Living class
 * All that lives shall be here!
 */
class Entity : public Updatable {

	/** position of this entity */
	Point pos_;

	/** something can carry this entity*/
    std::weak_ptr<Entity> carrier_;

	/** this entity can lift something */
    std::weak_ptr<Entity> lifted_;

    /** vision of this entity, could be a "wycięcie koła" */
    float visibilityRadius_ = 0;

public:
	Entity(World &world, Point pos);
	virtual ~Entity();

    Point getPos() const;
    void setPos(Point pos);

    void addCarrier(std::weak_ptr<Entity> e);
    void removeCarrier();
    std::weak_ptr<Entity> getCarrier() const;
    bool isCarried() const;

    void lift(std::weak_ptr<Entity> l);
    void unLift();
    std::weak_ptr<Entity> getLifted() const;
    bool isLifting() const;

    float getDistance(Entity * const e);

    float getVisibilityRadius() const;

protected:
    void setVisibilityRadius(float visibilityRadius);

public:
    template<typename Derived>
        std::vector<std::shared_ptr<Derived>> getVisible() {
            std::vector<std::shared_ptr<Derived>> m;
            auto v = this->getVisibleWithDistances<Derived>();
            for(auto p : v) {
                m.push_back(p.first);
            }
            return m;
        }

    template<typename Derived>
        std::map<std::shared_ptr<Derived>, float> getVisibleWithDistances();
};

/* predeclarations */
class Food;
class Anthill;
extern template std::map<std::shared_ptr<Food>, float> Entity::getVisibleWithDistances<Food>();
extern template std::map<std::shared_ptr<Anthill>, float> Entity::getVisibleWithDistances<Anthill>();


#endif // LIVING_H
