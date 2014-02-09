#ifndef Bat_hpp
#define Bat_hpp

#include "Gear.hpp"
#include "Player.hpp"

class Bat : Gear 
{
public:
    Bat(const int const strength, const int const precision);
	int getStrengthBonus () const;
	int getPrecisionBonus () const;
private:
	int const strengthBonus;
	int const precisionBonus;
};

#endif
