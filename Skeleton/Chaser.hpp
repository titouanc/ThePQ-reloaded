#ifndef Chaser_hpp
#define Chaser_hpp

#include "Player.hpp"

class Chaser : public Player 
{
public:
    Chaser();
	bool isChaser () const;
    int speed ();
    int collisionner ();
    int anticollision ();
    int pass ();
    int shoot ();
};

#endif
