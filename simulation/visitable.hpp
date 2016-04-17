#ifndef VISITABLE_HPP
#define VISITABLE_HPP

#include "visitor.hpp"
class Visitable
{
public:
    virtual void accept(Visitor& v) const = 0;
};

#endif // VISITABLE_HPP
