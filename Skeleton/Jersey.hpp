#ifndef Jersey_hpp
#define Jersey_hpp

#include "Gear.hpp"
#include "Player.hpp"

class Jersey : Gear 
{
public:
	int getStrengthBonus ();
	int getConstitutionBonus ();
	int getMagicBonus ();
	int getSpiritBonus ();
	void addStrength (Player aPlayer);
	void remStrength (Player aPlayer);
	void addConstitution (Player aPlayer);
	void removeConstitution (Player aPlayer);
	void addMagic (Player aPlayer);
	void removeMagic (Player aPlayer);
	void addSpirit (Player aPlayer);
	void removeSpirit (Player aPlayer);
private:
	int StrengthBonus;
	int ConstitutionBonus;
	int MagicBonus;
	int SpiritBonus;
};

#endif
