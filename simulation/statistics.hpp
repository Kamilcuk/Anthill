#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include "visitor.hpp"
#include "updatable.hpp"
#include <string>

class World;

class Statistics : public Visitor, public Updatable
{
    int antsAlive;
    World* world_;

public:
    Statistics(World* world);

    const std::string print() const;

    // Visitor interface
    void visit(const Ant &a);

    // Updatable interface
    void step(int s);
};

#endif // STATISTICS_HPP
