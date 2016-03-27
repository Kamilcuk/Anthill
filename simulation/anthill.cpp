#include "anthill.hpp"
#include "world.hpp"
#include "food.hpp"
#include "ant.hpp"
#include "entity.hpp"

Anthill::Anthill(World& world,Point pos) :
    Entity(world, pos)
{
    //this->setVisibilityRadius(0);
}

Anthill::~Anthill()
{
	/* dont delete food_ vector, couse it gets deleted in world */
	//this->food_.clear();
}

void Anthill::step()
{
	//if ( food_.size() > 10 ) {
    //    for(auto& f : food_) {
	//		f->setUsed(true);
	//	}
	//	new Ant(this->getWorld(), this->getPos().move(1,1));
	//	food_.clear();
	//}
}

void Anthill::lift(std::shared_ptr<Food> &f)
{
    //food_.push_back(std::shared_ptr<Food>(f));
    //this->Entity::lift(f);
}


void Anthill::lift(std::weak_ptr<Entity> &f)
{
    //if ( auto f1 = f.lock() ) {
    //    if ( auto f2 = std::dynamic_pointer_cast<Food>(f1) ) {
    //        this->lift(f2);
    //    }
    //}
}
