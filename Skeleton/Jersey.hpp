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
	int StrengthBonus;
	int ConstitutionBonus;
	int MagicBonus;
	int SpiritBonus;
};

#endif
