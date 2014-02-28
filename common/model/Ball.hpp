#ifndef __BALL_HPP
#define __BALL_HPP

#include "Moveable.hpp"
/*================Library defining different types of Moveables================*/
class Ball : public Moveable {
public:
	using Moveable::Moveable;
	bool isBall() const {return true;}
	virtual bool isQuaffle () const { return false; }
	virtual bool isBludger () const { return false; }
	virtual bool isGoldenSnitch () const { return false; }
	operator JSON::Dict(){
		JSON::Dict res = Moveable::operator JSON::Dict();
		res.set("name", getName());
		return res;
	}
};

class GoldenSnitch : public Ball {
/*Class creating the GoldenSnitch, inheriting from ball*/
public:
	using Ball::Ball;
	bool isGoldenSnitch () const { return true; }
	string getName() const { return "Golden Snitch"; }
};

class Bludger : public Ball {
/*Class creating a Bludger, inheriting from ball*/
public:
	using Ball::Ball;
	bool isBludger () const { return true; }
	string getName() const { return "Bludger"; }
	float collisionScore() const { return 20; } // TODO implement this
};

class Quaffle : public Ball {
/*Class creating Quaffle, inheriting from ball*/
public:
	using Ball::Ball;
	bool isQuaffle () const { return true; }
	string getName() const { return "Quaffle"; }
};


#endif
