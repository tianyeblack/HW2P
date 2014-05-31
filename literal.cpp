#include "literal.h"

Literal :: Literal(bool negated, unsigned int i, unsigned int j) {
	_negated = negated;
	_i = i;
	_j = j;
}

bool Literal :: getNegated() const {
	return _negated;
}

unsigned int Literal :: getI() const {
	return _i;
}

unsigned int Literal :: getJ() const {
	return _j;
}

bool Literal :: operator==(const Literal &l) const{
	if (_negated == l._negated && _i == l._i && _j == l._j) return true;
	else return false;
}

bool Literal :: weakEqual(const Literal &l) const {
	if (_i == l._i && _j == l._j) return true;
	else return false;
}
bool Literal :: operator<(const Literal &l) const {
	if (_i < l._i) return true;
	else if (_i > l._i) return false;
	else {
		if (_j < l._j) return true;
		else return false;
	}
}
