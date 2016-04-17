#ifndef VISITOR_HPP
#define VISITOR_HPP


class Ant;
class Visitor
{
public:
    virtual ~Visitor() {}
    virtual void visit(const Ant& a) = 0;
};

#endif // VISITED_HPP
