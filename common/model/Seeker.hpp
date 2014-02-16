#ifndef __SEEKER_HPP
#define __SEEKER_HPP

#include "Player.hpp"

class Seeker : public Player 
{
public:
    Seeker();
	bool isSeeker () const;
    int catchGS ();

};

#endif
