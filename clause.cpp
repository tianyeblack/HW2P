#include <iostream>
#include "clause.h"

using namespace std;

Clause :: Clause() {

}

void Clause :: copyFrom(Clause* tar) {
	for (set<Literal*, ltsvar> :: iterator it = tar->cnf.begin(); it != tar->cnf.end(); it++) {
		Literal* l = new Literal((*it)->getNegated(), (*it)->getI(), (*it)->getJ());
		cnf.insert(l);
	}	
}

set<Literal*, ltsvar>* Clause :: getLiterals() {
//	const set<Literal*, ltsvar>* ls = new set<Literal*, ltsvar>(cnf);
	return &cnf;
}

void Clause :: addLiteral(bool negated, unsigned int i, unsigned int j) {
	Literal* l = new Literal(negated, i, j);
	cnf.insert(l);
//	cout << "added " << (negated ? "!" : "") << "X" << i << j << endl;
}

bool Clause :: operator<(const Clause &l) const {
	if (cnf.size() < l.cnf.size()) return true;
	else if (cnf.size() > l.cnf.size()) return false;
	else {
		set<Literal*> :: iterator it1, it2;
		it1 = cnf.begin();
		it2 = l.cnf.begin();
		while (it1 != cnf.end() && it2 != l.cnf.end()) {
			if (*it1 < *it2) return true;
			else if (*it2 < *it1) return false;
			else if ((*it1)->getNegated() == true && (*it2)->getNegated() == false) return true;
			else if ((*it1)->getNegated() == false && (*it2)->getNegated() == true) return false;
			++it1;
			++it2;
		}
		return true;
	}
}

bool Clause :: operator==(const Clause &l) const {
	if (cnf.size() != l.cnf.size()) return false;
	else {
		set<Literal*> :: iterator it1, it2;
		it1 = cnf.begin();
		it2 = l.cnf.begin();
		while (it1 != cnf.end() && it2 != l.cnf.end()) {
			if (!(**it1 == **it2)) return false;
			++it1;
			++it2;
		}
		return true;
	}
}

bool Clause :: satisfiedBy(bool** model) const {
	for (set<Literal*, ltsvar> :: iterator it = cnf.begin(); it != cnf.end(); it++) {
		if ((*it)->getNegated() != model[(*it)->getI()][(*it)->getJ()]) return true;
	}
	return false;
}

void Clause :: printClause() const{
	for (set<Literal*, ltsvar> :: iterator it = cnf.begin(); it != cnf.end(); it++) {
		cout << ((*it)->getNegated() ? "!" : "") << "X" << (*it)->getI() << (*it)->getJ() << " V ";
	}
	cout << endl;
}
