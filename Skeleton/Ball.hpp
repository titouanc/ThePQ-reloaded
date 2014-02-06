#ifndef Ball_hpp
#define Ball_hpp

#include "Moveable.hpp"

class Ball : Moveable {
public:
	bool isQuaffle ();
	bool isBludger ();
	bool isGoldenSnitch ();
};

#endif