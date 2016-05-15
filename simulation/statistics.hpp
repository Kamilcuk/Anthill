#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include "visitor.hpp"
#include "entity.hpp"
#include <string>
#include <vector>
class Visitable;

/**
 * The Statistics
 */
class Statistics : public Visitor
{
public:
	/**
	 * Counts all the living and all the dead of one object type
	 */
	struct EntityCnt {
		/** how many object are alive */
		unsigned int existing = 0;
		/** how many object were removed - integral from 0 to step from (existed-existing)  */
		unsigned int removed = 0;
		/** how many objects were there on the beggining */
		unsigned int init = 0;

		EntityCnt();
		void prepare();
		void count(const Entity& e, bool &firstCount);
	};
private:

	/** is this first time we count objects? */
	bool firstCount_ = true;
	/** step number */
	unsigned int stepNumber_ = 0;

	/** ants counters */
	EntityCnt antCnt_;
	/** foods counters */
	EntityCnt foodCnt_;
	/** obstacle counter */
	EntityCnt obstacleCnt_;

	/** how many ants are carrying food ? */
	unsigned int antsCarring_;

public:
	Statistics();

	/**
	 * simple print method for testing
	 */
    const std::string print() const;

	// refreshes all stats, using vector of visitables
	void update(const std::vector<Visitable *> &vv);

	// Visitor interface
	void visit(const Ant &a);
	void visit(const Food &f);
	void visit(const AntMandibles &u);
	void visit(const Obstacle &u);
	EntityCnt antCnt() const;
	EntityCnt foodCnt() const;
	EntityCnt obstacleCnt() const;
	unsigned int stepNumber() const;
};

#endif // STATISTICS_HPP
