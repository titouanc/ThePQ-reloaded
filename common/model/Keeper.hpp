#ifndef __KEEPER_HPP
#define __KEEPER_HPP

#include "Player.hpp"

class Keeper : public Player 
{
public:
    Keeper();
	bool isKeeper ();
    int catchBall ();
    int pass ();
};

#endif
