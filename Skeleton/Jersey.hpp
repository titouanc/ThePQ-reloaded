#ifndef Jersey_hpp
#define Jersey_hpp

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
	int strengthBonus;
	int constitutionBonus;
	int magicBonus;
	int spiritBonus;
};

#endif
