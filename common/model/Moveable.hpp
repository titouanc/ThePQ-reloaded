#ifndef __MOVEABLE_HPP
#define __MOVEABLE_HPP

#include <string>
#include "Position.hpp"

using namespace std;

class Moveable {
public:
	// CONSTRUCTORS
	Moveable(): _name("NoName"), _uniqueID(0), _speed(0), _position(0, 0){}
	Moveable(string name, unsigned int id, float speed, Position &position): 
				_name(name), _uniqueID(id), _speed(speed), _position(position){}
	Moveable(JSON::Dict const & json) : Moveable() {
		if (ISSTR(json.get("name"))) _name = STR(json.get("name")).value();
		if (ISINT(json.get("ID"))) _uniqueID = INT(json.get("ID")).value();
		if (ISLIST(json.get("position"))) _position = Position(LIST(json.get("position")));
		if (ISFLOAT(json.get("speed"))) _speed = FLOAT(json.get("speed")).value();
		else if (ISINT(json.get("speed"))) _speed = INT(json.get("speed")).value();
	}
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

	JSON::Dict toJson() const {
		JSON::Dict res;
		res.set("speed", _speed);
		res.set("position", _position.toJson());
		res.set("ID", _uniqueID);
		res.set("name", _name);
		return res;
	}

private:
	string _name;
	unsigned int _uniqueID;
	double _speed;
	Position _position;
};

#endif

