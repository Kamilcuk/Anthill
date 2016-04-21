#include "statistics.hpp"
#include "world.hpp"
#include "visitable.hpp"
#include <iostream>

Statistics::Statistics(World* world) : Updatable(world), antsAlive(0),
    world_(world)
{
}

const std::string Statistics::print() const
{
    std::string ret = "";
    ret = "Number ants alive: " + std::to_string(this->antsAlive);
    std::cout << ret << std::endl;
    return ret;
}

void Statistics::visit(const Ant &a)
{
    antsAlive++;
}

void Statistics::step(int s)
{
    if ( !s ) return;
    antsAlive = 0;
    auto vv = world_->getVisitablePtrs();
    std::cout << "num visitable ptrs " << vv.size() << std::endl;
    for (auto& v : vv) {
        v->accept(*this);
    }
}
