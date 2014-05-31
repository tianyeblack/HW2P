#pragma once

class Literal {
protected:
	bool _negated;
	unsigned int _i;
	unsigned int _j;
public:
	Literal(bool negated, unsigned int i, unsigned int j);
	bool getNegated() const;
	unsigned int getI() const;
	unsigned int getJ() const;
	bool operator==(const Literal &l) const;
	bool operator<(const Literal &l) const;
	bool weakEqual(const Literal &l) const;
};
