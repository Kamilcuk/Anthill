#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include "updatable.hpp"
#include "visitor.hpp"
#include <string>
#include <vector>
class Visitable;

/**
 * Counts the living and the dead of an object
 */
struct UpdatableCount {
	/** how many object are alive */
	unsigned int existing = 0;
	/** how many object were removed */
	unsigned int removed = 0;
	/** how many objects were there on the beggining */
	unsigned int init = 0;

	void reset() {
		existing = removed = init = 0;
	}
};

/**
 * The Statistics
 */
class Statistics : public Visitor
{
	/** step number */
	unsigned int stepNumber_;

	/** ants counters */
	UpdatableCount antCnt_;
	/** foods counters */
	UpdatableCount foodCnt_;
	/** obstacle counter */
	UpdatableCount obstacleCnt_;

	/** how many ants are carrying food ? */
	unsigned int antsCarringFood_;

public:
	Statistics();

	/**
	 * simple print method for testing
	 */
    const std::string print() const;

	// refreshes all stats, using vector of visitables
	void update(const std::vector<Visitable *> &vv);

	// counts instances of updatable
	void count(UpdatableCount& c, const Updatable& u);

	// resets all the counts
	void reset();

	// Visitor interface
	void visit(const Ant &a);
	void visit(const Food &f);
	void visit(const AntMandibles &u);
	void visit(const Obstacle &u);
};

#endif // STATISTICS_HPP
