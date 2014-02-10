#ifndef MOVEABLE_HPP
#define MOVEABLE_HPP

#include "Constants.hpp"

class Moveable {
public:
	// CONSTRUCTORS
	Moveable();
	Moveable(string name, unsigned int id, float speed);
	~Moveable();

	// GETTERS + SETTERS
	Position getPosition();
	void setPosition();

	float getSpeed();
	void setSpeed();

	unsigned int getID();
	void setID()

private:
	string _name;
	unsigned int _uniqueID;
	float _speed;
};

#endif

