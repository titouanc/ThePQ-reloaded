#ifndef Moveable_hpp
#define Moveable_hpp

#include "Constants.hpp"
#include "Position.hpp"

class Moveable {
public:
	// CONSTRUCTORS
	Moveable();
	Moveable(std::string name, unsigned int id, float speed);
	~Moveable();

	// GETTERS + SETTERS
	Position getPosition();
	void setPosition();

	float getSpeed();
	void setSpeed();

	unsigned int getID();
	void setID();

private:
    std::string _name;
	unsigned int _uniqueID;
	float _speed;
};

#endif

