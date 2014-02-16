#ifndef __GEAR_HPP
#define __GEAR_HPP

#include <string>

using namespace std;

class Gear 
{
public:
	Gear(string name, string description, int price): 	_name(name), 
														_description(description),
														_price(price) {}
    string getName () const 		{ return _name; }
    string getDescription () const	{ return _description; }
	int getPrice () const;			{ return _price; }
private:
    string const _name;
    string const _description;
	int const _price;
};


// BAT -------------------------------------------------------------------------
class Bat : Gear 
{
public:
    Bat(const int const strength, const int const precision) : 	_strengthBonus(strength),
    															_precisionBonus(precision){}
	int getStrengthBonus () const	{ return _strengthBonus; }
	int getPrecisionBonus () const	{ return _precisionBonus; }
private:
	int const _strengthBonus;
	int const _precisionBonus;
};


// BROOMSTICK ------------------------------------------------------------------
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


// JERSEY ----------------------------------------------------------------------
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
