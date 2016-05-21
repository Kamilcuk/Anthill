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
    virtual void visit(const Ant& a) {};
	virtual void visit(const Food& f) {};
	virtual void visit(const AntMandibles& u) {};
	virtual void visit(const Obstacle& u) {};
};

#endif // VISITED_HPP
