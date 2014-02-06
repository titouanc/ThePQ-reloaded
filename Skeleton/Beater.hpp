#ifndef Beater_hpp
#define Beater_hpp

#include "Player.hpp"
#include "Bat.hpp"

class Beater : Player 
{
public:
	bool isBeater ();
	void equipBat (Bat aBat);
private:
	Bat bat;
};

#endif
