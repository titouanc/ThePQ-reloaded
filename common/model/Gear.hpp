#ifndef __GEAR_HPP
#define __GEAR_HPP

#include <string>
#include <json/json.hpp>

using namespace std;

class Gear 
{
public:
	Gear(): _name("NoName"), 
			_description("NoDescription"),
			_price(0) {}
	Gear(JSON::Dict const &json): Gear() {
		if (ISSTR(json.get("name"))) _name = STR(json.get("name")).value();
		if (ISSTR(json.get("description"))) _description = STR(json.get("description")).value();
		if (ISINT(json.get("price"))) _price = INT(json.get("price")).value();		
	}
	operator JSON::Dict(){
		JSON::Dict res;
		res.set("name", _name);
		res.set("description", _description);
		res.set("price", _price);
		return res;
	}
    string getName () const 		{ return _name; }
    string getDescription () const	{ return _description; }
	int getPrice () const			{ return _price; }
	void setName(string name)				{ _name = name; }
	void setDescription(string description)	{ _description = description; }
	void setPrice(int price)				{ _price = price; }
private:
    string  _name;
    string  _description;
	int  _price;
};


// BAT -------------------------------------------------------------------------
class Bat : Gear 
{
public:
    Bat(const int strength, const int precision) : 	_strengthBonus(strength),
    												_precisionBonus(precision){}
    Bat(JSON::Dict const &json): Gear(json) {
		if (ISINT(json.get("strengthBonus"))) _strengthBonus = INT(json.get("strengthBonus")).value();		
		if (ISINT(json.get("precisionBonus"))) _precisionBonus = INT(json.get("precisionBonus")).value();		
	}
	operator JSON::Dict(){
		JSON::Dict res = JSON::Dict((Gear)*this);
		res.set("strengthBonus", _strengthBonus);
		res.set("precisionBonus", _precisionBonus);
		return res;
	}												
	int getStrengthBonus () const	{ return _strengthBonus; }
	int getPrecisionBonus () const	{ return _precisionBonus; }
private:
	int _strengthBonus;
	int _precisionBonus;
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
	int _cases;
	int _velocityBonus;
};


// JERSEY ----------------------------------------------------------------------
class Jersey : Gear 
{
public:
	Jersey():_strengthBonus(0), _constitutionBonus(0), _magicBonus(0), _spiritBonus(0){}

	int getStrengthBonus () const 		{ return _strengthBonus; }
	int getConstitutionBonus () const 	{ return _constitutionBonus; }
	int getMagicBonus () const 			{ return _magicBonus; }
	int getSpiritBonus () const 		{ return _spiritBonus; }
private:
	int _strengthBonus;
	int _constitutionBonus;
	int _magicBonus;
	int _spiritBonus;
};

#endif
