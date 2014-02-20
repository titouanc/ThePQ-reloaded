#ifndef __BALL_HPP
#define __BALL_HPP

#include "Moveable.hpp"
/*================Library defining different types of Moveables================*/
class Ball : public Moveable {
public:
	bool isBall() const {return true;}
	virtual bool isQuaffle () { return false; }
	virtual bool isBludger () { return false; }
	virtual bool isGoldenSnitch () { return false; }
};

class GoldenSnitch : public Ball {
/*Class creating the GoldenSnitch, inheriting from ball*/
public:
	bool isGoldenSnitch () { return true; }
	string getName() { return "Golden Snitch"; }
};

class Bludger : public Ball {
/*Class creating a Bludger, inheriting from ball*/
public:
	bool isBludger () { return true; }
	string getName() { return "Bludger"; }
	float collisionScore() { return 20; } // TODO implement this
};

class Quaffle : public Ball {
/*Class creating Quaffle, inheriting from ball*/
public:
	bool isQuaffle () { return true; }
	string getName() { return "Quaffle"; }
};


#endif