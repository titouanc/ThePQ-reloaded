#ifndef MOVEABLE_HPP
#define MOVEABLE_HPP

#include <string>
#include "Constants.hpp"
#include "position.h"

class Moveable {
public:
	// CONSTRUCTORS
	Moveable(){}
	Moveable(string name, unsigned int id, float speed, Position position): 
				_name(name), _uniqueID(id), _speed(speed), _position(position);
	~Moveable();

	// GETTERS + SETTERS
	Position getPosition() { return _position; }
	void setPosition(Position position) { _position = position; }

	float getSpeed() { return speed; }
	void setSpeed(float speed) { _speed = speed; }

	unsigned int getID() { return _uniqueID; }
	void setID(unsigned int newID) { _uniqueID = newID; }

private:
	string _name;
	unsigned int _uniqueID;
	float _speed;
	Position _position;
};

#endif

