#ifndef VISITOR_HPP
#define VISITOR_HPP


class Ant;
class Food;
class AntMandibles;
class Obstacle;

/**
 * Represents visitors, who will visit.
 */
class Visitor
{
public:
    virtual ~Visitor() {}
    virtual void visit(const Ant& a) = 0;
	virtual void visit(const Food& f) = 0;
	virtual void visit(const AntMandibles& u) = 0;
	virtual void visit(const Obstacle& u) = 0;
};

#endif // VISITED_HPP
