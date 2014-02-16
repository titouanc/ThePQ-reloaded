#ifndef __BROOMSTICK_HPP
#define __BROOMSTICK_HPP

#include "Gear.hpp"

class Broomstick : Gear 
{
public:
    Broomstick(const int nCases, const int velocity):	_cases(nCases), 
    													_velocityBonus(velocity){}
	int getCases () const 			{ return _cases; }
	int getVelocityBonus () const  	{ return _velocityBonus; }
private:
	const int _cases;
	const int _velocityBonus;
};

#endif
