#ifndef Keeper_hpp
#define Keeper_hpp

#include "Player.hpp"

class Keeper : public Player 
{
public:
	bool isKeeper ();
    int catchBall ();
    int pass ();
};

#endif
