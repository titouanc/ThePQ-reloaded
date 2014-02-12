#ifndef MOVEABLE_HPP
#define MOVEABLE_HPP

#include <string>
#include "position.h"

using namespace std;

class Moveable {
public:
	// CONSTRUCTORS
	Moveable(): _name("NoName"), _uniqueID(0), _speed(0), _position(Position(0,0)){}
	Moveable(string name, unsigned int id, float speed, Position &position): 
				_name(name), _uniqueID(id), _speed(speed), _position(position){}
	~Moveable(){}

	// GETTERS + SETTERS
	string getName() const { return _name; }
	void setName(string name) { _name = name; }

	Position getPosition() const { return _position; }
	void setPosition(Position &position) { _position = position; }

	float getSpeed() const { return _speed; }
	void setSpeed(float speed) { _speed = speed; }

	unsigned int getID() const { return _uniqueID; }
	void setID(unsigned int newID) { _uniqueID = newID; }

private:
	string _name;
	unsigned int _uniqueID;
	float _speed;
	Position _position;
};

#endif

