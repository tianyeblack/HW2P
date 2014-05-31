#include <set>
#include "literal.h"

using namespace std;

struct ltsvar {
	bool operator() (const Literal *l1, const Literal *l2) const {
		if (l1->getI() < l2->getI()) return true;
		else if (l1->getI() > l2->getI()) return false;
		else {
			if (l1->getJ() < l2->getJ()) return true;
			else return false;
		}
	}
};

class Clause {
public:
	set<Literal*, ltsvar> cnf;
	Clause();
	void copyFrom(Clause* tar);
	void addLiteral(bool negated, unsigned int i, unsigned int j);
	bool operator==(const Clause &c) const;
	bool operator<(const Clause &l) const;
	bool satisfiedBy(bool** model) const;
	set<Literal*, ltsvar>* getLiterals();
	void printClause() const;
};
