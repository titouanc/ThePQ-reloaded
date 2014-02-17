#ifndef __BALL_HPP
#define __BALL_HPP

#include "Moveable.hpp"

class Ball : public Moveable {
public:
	virtual bool isQuaffle () { return false; }
	virtual bool isBludger () { return false; }
	virtual bool isGoldenSnitch () { return false; }
};

class GoldenSnitch : public Ball {
public:
	bool isGoldenSnitch () { return true; }
	string getName() { return "Golden Snitch"; }
};

class Bludger : public Ball {
public:
	bool isBludger () { return true; }
	string getName() { return "Bludger"; }
};

class Quaffle : public Ball {
public:
	bool isQuaffle () { return true; }
	string getName() { return "Quaffle"; }
};


#endif