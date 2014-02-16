#ifndef Seeker_hpp
#define Seeker_hpp

#include "Player.hpp"

class Seeker : public Player 
{
public:
	bool isSeeker () const;
    int catchGS ();

};

#endif
