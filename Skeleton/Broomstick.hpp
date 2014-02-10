#ifndef Broomstick_hpp
#define Broomstick_hpp

#include "Gear.hpp"
#include "Player.hpp"

class Broomstick : Gear 
{
public:
    Broomstick(const int const nCases, const int const velocity);
	int getCases () const ;
	int getVelocityBonus () const ;
private:
	int const cases;
	int const velocityBonus;
};

#endif
