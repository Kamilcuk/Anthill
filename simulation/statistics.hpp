#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include "visitor.hpp"
#include "updatable.hpp"
#include <ostream>

class Statistics : public Visitor, public Updatable
{
    int antsAlive;

public:
    Statistics(World& world);

    void print(std::ostream& out);

    // Visitor interface
public:
    void visit(const Ant &a);

    // Updatable interface
public:
    void step(int);
};

#endif // STATISTICS_HPP
