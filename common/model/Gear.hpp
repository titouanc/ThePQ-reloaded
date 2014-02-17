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
class Bat : public Gear 
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
class Broomstick : public Gear 
{
public:
    Broomstick(const int nCases, const int velocity):	_cases(nCases), 
    													_velocityBonus(velocity){}
	Broomstick(JSON::Dict const &json): Gear(json) {
		if (ISINT(json.get("cases"))) _cases = INT(json.get("cases")).value();		
		if (ISINT(json.get("velocityBonus"))) _velocityBonus = INT(json.get("velocityBonus")).value();		
	}
	operator JSON::Dict(){
		JSON::Dict res = JSON::Dict((Gear)*this);
		res.set("cases", _cases);
		res.set("velocityBonus", _velocityBonus);
		return res;
	}	
	int getCases () const 			{ return _cases; }
	int getVelocityBonus () const  	{ return _velocityBonus; }
private:
	int _cases;
	int _velocityBonus;
};


// JERSEY ----------------------------------------------------------------------
class Jersey : public Gear 
{
public:
	// TODO : we have to be able to set all the attributes of Jersey
	Jersey():_strengthBonus(0), _constitutionBonus(0), _magicBonus(0), _spiritBonus(0){}

	Jersey(JSON::Dict const &json): Gear(json) {
		if (ISINT(json.get("strengthBonus"))) _strengthBonus = INT(json.get("strengthBonus")).value();		
		if (ISINT(json.get("constitutionBonus"))) _constitutionBonus = INT(json.get("constitutionBonus")).value();		
		if (ISINT(json.get("magicBonus"))) _magicBonus = INT(json.get("magicBonus")).value();		
		if (ISINT(json.get("spiritBonus"))) _spiritBonus = INT(json.get("spiritBonus")).value();		
	}
	operator JSON::Dict(){
		JSON::Dict res = JSON::Dict((Gear)*this);
		res.set("strengthBonus", _strengthBonus);
		res.set("constitutionBonus", _constitutionBonus);
		res.set("magicBonus", _magicBonus);
		res.set("spiritBonus", _spiritBonus);
		return res;
	}	
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
