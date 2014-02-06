#ifndef Stroke_hpp
#define Stroke_hpp 

#include "Action.hpp"

class Stroke {
public:
	Stroke();
	~Stroke();

	Position getPositionAtTime(float t);
private:
	Player _player;
	Action _actions[];
};

#endif