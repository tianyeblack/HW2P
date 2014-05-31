#include <iostream>
#include <cstdlib>
#include <ctime>
#include "probleminstance.h"

using namespace std;

ProblemInstance :: ProblemInstance(unsigned int chemicals, unsigned int containers, int** csrt_tab) {
	_chemicals = chemicals;
	_containers = containers;
	for (unsigned int i = 0; i < chemicals; i++) {
		Clause* atleastone = new Clause;
		for (unsigned int j = 0; j < containers; j++) {
			atleastone->addLiteral(false, i, j);
		}
//		cout << "Added AtLeastOne" << endl;
//		atleastone->printClause();
		_kb.insert(atleastone);
		
		for (unsigned int j = 0; j < containers - 1; j++) {
//			cout << "Added AtMostOne" << endl;
			for (unsigned int k = j + 1; k < containers; k++) {
				Clause* atmostone = new Clause;
				atmostone->addLiteral(true, i, j);
				atmostone->addLiteral(true, i, k);
//				atmostone->printClause();
				_kb.insert(atmostone);
			}
		}
		
		for (unsigned int k = i + 1; k < chemicals; k++) {
			if (-1 == csrt_tab[i][k]) {
//				cout << "Added separate" << endl;
				for (unsigned int j = 0; j < containers; j++) {
					Clause* separate = new Clause;
					separate->addLiteral(true, i, j);
					separate->addLiteral(true, k, j);
//					separate->printClause();
					_kb.insert(separate);
				}
			} else if (1 == csrt_tab[i][k]) {
//				cout << "Added together" << endl;
				for (unsigned int j = 0; j < containers; j++) {
					Clause* together1 = new Clause;
					together1->addLiteral(true, i, j);
					together1->addLiteral(false, k, j);
//					together1->printClause();
					_kb.insert(together1);
					Clause* together2 = new Clause;
					together2->addLiteral(false, i, j);
					together2->addLiteral(true, k, j);
//					together2->printClause();
					_kb.insert(together2);
				}
			}
		}
	}
//	for (set<Clause*, ltcl> :: iterator it = _kb.begin(); it != _kb.end(); it++) (*it)->printClause();
}

int ProblemInstance :: kbSize() {
	return _kb.size();
}

bool ProblemInstance :: plResolution() {
	set<Clause*, ltcl>* new_kb = new set<Clause*, ltcl>();
	for (set<Clause*, ltcl> :: iterator it = _kb.begin(); it != _kb.end(); it++) {
		Clause* copy = new Clause;
		copy->copyFrom(*it);
		new_kb->insert(copy);
	}
	/*
	for (set<Clause*, ltcl> :: iterator it = new_kb->begin(); it != new_kb->end(); it++) {
		(*it)->printClause();
	}
	*/
	set<Clause*, ltcl>* new_clauses = new set<Clause*, ltcl>();
//	cout << new_kb->size() << " " << new_clauses->size() << endl;
	bool added = true;
	while (added) {
		added = false;
		for (set<Clause*, ltcl> :: iterator it1 = new_kb->begin(); it1 != new_kb->end(); it1++) {
			for (set<Clause*, ltcl> :: iterator it2 = new_kb->begin(); it2 != new_kb->end(); it2++) {
				if (!(**it2 == **it1)) {
//					(*it1)->printClause();
//					cout << "And\n";
//					(*it2)->printClause();
//					cout << endl;
					Clause* resolvent = plResolve(*it1, *it2);
					if (!resolvent) {
						return false;
					} else {
						if (resolvent->getLiterals()->size() != 0 && hasClause(new_clauses, resolvent) == false) new_clauses->insert(resolvent);
						else delete resolvent;
					}
				}
			}
		}
		for (set<Clause*, ltcl> :: iterator it = new_clauses->begin(); it != new_clauses->end(); it++) {
			if (hasClause(new_kb, *it) == false) {
				added = true;
				Clause* copy = new Clause;
				copy->copyFrom(*it);
				new_kb->insert(copy);
			}
		}
//		cout << new_clause->size() << endl;
	}
	for (set<Clause*, ltcl> :: iterator it = new_kb->begin(); it != new_kb->end(); it++) {
		(*it)->getLiterals()->clear();
	}
	for (set<Clause*, ltcl> :: iterator it = new_clauses->begin(); it != new_clauses->end(); it++) {
		(*it)->getLiterals()->clear();
	}
	new_kb->clear();
	new_clauses->clear();
	delete new_kb;
	delete new_clauses;
	return true;
}

Clause* ProblemInstance :: plResolve(Clause *c1, Clause *c2) {
//	c1->printClause();
//	c2->printClause();
	Clause* resolvent = new Clause;
	int complimentary = 0;
	const set<Literal*, ltsvar>* s1 = c1->getLiterals();
	const set<Literal*, ltsvar>* s2 = c2->getLiterals();
	set<Literal*, ltsvar> :: iterator it1 = s1->begin();
	set<Literal*, ltsvar> :: iterator it2 = s2->begin();
	while (it1 != s1->end() || it2 != s2->end()) {
		if (it2 == s2->end()) {
			resolvent->addLiteral((*it1)->getNegated(), (*it1)->getI(), (*it1)->getJ());
			it1++;
		} else if (it1 == s1->end()) {
			resolvent->addLiteral((*it2)->getNegated(), (*it2)->getI(), (*it2)->getJ());
			it2++;
		} else if (**it1 < **it2) {
			resolvent->addLiteral((*it1)->getNegated(), (*it1)->getI(), (*it1)->getJ());
			it1++;
		} else if (**it2 < **it1) {
			resolvent->addLiteral((*it2)->getNegated(), (*it2)->getI(), (*it2)->getJ());
			it2++;
		} else if ((**it1).weakEqual(**it2) == true) {
			if ((*it1)->getNegated() == (*it2)->getNegated()) {
				resolvent->addLiteral((*it1)->getNegated(), (*it1)->getI(), (*it1)->getJ());
			} else {
				complimentary++;
			}
			it1++;
			it2++;
		} else {
			delete resolvent;
			cout << "error" << endl;
			return 0;
		}
	}
	if (complimentary > 1 || complimentary == 0) {
		delete resolvent;
//		resolvent->addLiteral(false, _chemicals, _containers);
		return new Clause;
	} else {
		if (resolvent->getLiterals()->size() == 0) {
			delete resolvent;
			return 0;
		} else {
			return resolvent;
		}
	}
}

bool ProblemInstance :: hasClause(const set<Clause*, ltcl>* clauses, const Clause *c) const {
	for (set<Clause*, ltcl> :: iterator it = clauses->begin(); it != clauses->end(); it++) {
		if (**it == *c) return true;
	}
	return false;
}

bool** ProblemInstance :: walkSAT(double p, unsigned int max_flips, int *flips) {
	srand(time(0));
	bool** model = new bool*[_chemicals];
	for (unsigned int i = 0; i < _chemicals; i++) {
		model[i] = new bool[_containers];
		for (unsigned int j = 0; j < _containers; j++) {
			model[i][j] = (bool) rand() % 2;
		}
	}
//	for (unsigned int i = 0; i < _chemicals; i++) solution[i] = rand() % _containers;
	for (unsigned int m = 0; m < max_flips; m++) {
		if (satisfiesClauses(&_kb, model) == _kb.size()) {
			*flips = m + 1;
			return model;
		}
		Clause* unsat;
		while (1) {
			set<Clause*, ltcl> :: iterator it = _kb.begin();
			advance(it, rand() % _kb.size());
			if ((*it)->satisfiedBy(model) == false) {
				unsat = *it;
				break;
			}
		}
		if (p > 0) {
			if (p == 1.0 || (double) rand() / (double) RAND_MAX > p) {
				const set<Literal*, ltsvar>* ls = unsat->getLiterals();
				set<Literal*, ltsvar> :: iterator it = ls->begin();
				advance(it, rand() % ls->size());
				if (model[(*it)->getI()][(*it)->getJ()] == true) model[(*it)->getI()][(*it)->getJ()] = false;
				else model[(*it)->getI()][(*it)->getJ()] = true;
			}
		} else {
			const set<Literal*, ltsvar>* ls = unsat->getLiterals();
			Literal* l;
			unsigned int max_sat = 0;
			for (set<Literal*, ltsvar> :: iterator it = ls->begin(); it != ls->end(); it++) {
				bool** new_model = new bool*[_chemicals];
				for (unsigned int i = 0; i < _chemicals; i++) {
					new_model[i] = new bool[_containers];
					for (unsigned int j = 0; j < _containers; j++) {
						new_model[i][j] = model[i][j];
					}
				}
				if (new_model[(*it)->getI()][(*it)->getJ()] == true) new_model[(*it)->getI()][(*it)->getJ()] = false;
				else new_model[(*it)->getI()][(*it)->getJ()] = true;
				if (satisfiesClauses(&_kb, new_model) > max_sat) l = *it;
				delete[] new_model;
			}
			if (model[l->getI()][l->getJ()] == true) model[l->getI()][l->getJ()] = false;
			else model[l->getI()][l->getJ()] = true;
		}
	}
	delete[] model;
	return 0;
}

unsigned int ProblemInstance :: satisfiesClauses(const set<Clause*, ltcl>* clauses, bool **model) const {
	unsigned int sat = 0;
	for (set<Clause*, ltcl> :: iterator it = clauses->begin(); it != clauses->end(); it++) {
		if ((*it)->satisfiedBy(model) == true) sat++;;
	}
	return sat;
}
