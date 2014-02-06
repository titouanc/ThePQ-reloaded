#ifndef Bat_hpp
#define Bat_hpp

#include "Gear.hpp"
#include "Player.hpp"

class Bat : Gear 
{
public:
	int getStrengthBonus ();
	int getPrecisionBonus ();
	void addStrength (Player aPlayer);
	void removeStrength (Player aPlayer);
	void addPrecision (Player aPlayer);
	void removePrecision (Player aPlayer);
private:
	int StrengthBonus;
	int PrecisionBonus;
};

#endif
