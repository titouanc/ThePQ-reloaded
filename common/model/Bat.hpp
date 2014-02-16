#ifndef __BAT_HPP
#define __BAT_HPP

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
