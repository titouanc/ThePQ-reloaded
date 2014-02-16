#ifndef __JERSEY_HPP
#define __JERSEY_HPP

#include "Gear.hpp"
#include "Player.hpp"

class Jersey : Gear 
{
public:
	int getStrengthBonus () const;
	int getConstitutionBonus () const;
	int getMagicBonus () const;
	int getSpiritBonus () const;
private:
    Jersey();
	int strengthBonus;
	int constitutionBonus;
	int magicBonus;
	int spiritBonus;
};

#endif
