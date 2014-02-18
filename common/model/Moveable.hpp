#ifndef __MOVEABLE_HPP
#define __MOVEABLE_HPP

#include <string>
#include "Position.hpp"

using namespace std;

class Moveable {
public:
	// CONSTRUCTORS
	Moveable():  _uniqueID(0), _speed(0), _position(0, 0){}
	Moveable(unsigned int id, float speed, Position &position): 
				_uniqueID(id), _speed(speed), _position(position){}
	Moveable(JSON::Dict const & json) : Moveable() {
		if (ISINT(json.get("ID"))) _uniqueID = INT(json.get("ID")).value();
		if (ISLIST(json.get("position"))) _position = Position(LIST(json.get("position")));
		if (ISFLOAT(json.get("speed"))) _speed = FLOAT(json.get("speed")).value();
		else if (ISINT(json.get("speed"))) _speed = INT(json.get("speed")).value();
	}
	~Moveable(){}

	virtual string getName() { return ""; }

	Position getPosition() const { return _position; }
	void setPosition(Position const & position) { _position = position; }

	float getSpeed() const { return _speed; }
	void setSpeed(float speed) { _speed = speed; }

	unsigned int getID() const { return _uniqueID; }
	void setID(unsigned int newID) { _uniqueID = newID; }

	virtual float collisionScore() { return 0; }

	operator JSON::Dict() const {
		JSON::Dict res;
		res.set("speed", _speed);
		res.set("position", _position.toJson());
		res.set("ID", _uniqueID);
		return res;
	}

private:
	unsigned int _uniqueID;
	double _speed;
	Position _position;
};

#endif

