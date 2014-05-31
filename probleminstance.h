#pragma once

#include <set>
#include "clause.h"

using namespace std;

struct eqcl {
	bool operator()(const Clause *c1, const Clause *c2) const {
		return *c1 == *c2;
	}
};

struct ltcl {
	bool operator()(const Clause *c1, const Clause *c2) const {
		return *c1 < *c2;
	}
};

class ProblemInstance {
private:
	set<Clause*, ltcl> _kb;
	unsigned int _chemicals;
	unsigned int _containers;
	bool hasClause(const set<Clause*, ltcl>* clauses, const Clause *c) const;
	unsigned int satisfiesClauses(const set<Clause*, ltcl>* clauses, bool **model) const;
public:
	int kbSize();
	ProblemInstance(unsigned int chemicals, unsigned int containers, int** csrt_tab);
	bool plResolution();
	Clause* plResolve(Clause *c1, Clause *c2);
	bool** walkSAT(double p, unsigned int max_flips, int *flips);
};
