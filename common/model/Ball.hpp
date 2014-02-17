#ifndef __BALL_HPP
#define __BALL_HPP

#include "Moveable.hpp"

class Ball : Moveable {
public:
	virtual bool isQuaffle () { return false; }
	virtual bool isBludger () { return false; }
	virtual bool isGoldenSnitch () { return false; }
};

class GoldenSnitch : Ball {
public:
	bool isGoldenSnitch () { return true; }
};

class Bludger : Ball {
public:
	bool isBludger () { return true; }
};

class Quaffle : Ball {
public:
	bool isQuaffle () { return true; }
};


#endif