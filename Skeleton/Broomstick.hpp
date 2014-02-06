#ifndef Broomstick_hpp
#define Broomstick_hpp

#include "Gear.hpp"
#include "Player.hpp"

class Broomstick : Gear 
{
public:
	int getCases ();
	int getVelocityBonus ();
	void addVelocity (Player aPlayer);
	void removeVelocity (Player aPlayer);
private:
	int cases;
	int velocityBonus;
};

#endif
