#include "statistics.hpp"

Statistics::Statistics(World &world) : Updatable(world)
{

}

void Statistics::visit(const Ant &a)
{
    antsAlive++;
}

void Statistics::step(int)
{
    antsAlive = 0;
}
