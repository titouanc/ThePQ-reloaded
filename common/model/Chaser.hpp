#ifndef __CHASER_HPP
#define __CHASER_HPP

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
