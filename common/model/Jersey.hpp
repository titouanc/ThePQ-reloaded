#ifndef __JERSEY_HPP
#define __JERSEY_HPP

#include "Gear.hpp"

class Jersey : Gear 
{
public:
	Jersey():strengthBonus(0), constitutionBonus(0), magicBonus(0), spiritBonus(0){}

	int getStrengthBonus () const 		{ return _strengthBonus; }
	int getConstitutionBonus () const 	{ return _; }
	int getMagicBonus () const 			{ return _magicBonus; }
	int getSpiritBonus () const 		{ return _spiritBonus; }
private:
    Jersey();
	int _strengthBonus;
	int _constitutionBonus;
	int _magicBonus;
	int _spiritBonus;
};

#endif
