#ifndef __GEAR_HPP
#define __GEAR_HPP

#include <string>
#include <json/json.hpp>

using namespace std;
/*========================Class creating objects used by Player========================*/
class Gear 
{
public:
	Gear(): _name("NoName"), 
			_description("NoDescription"),
			_price(0) {}
	Gear(string name, string descr, int price): _name(name), _description(descr), _price(price){}//modif
	Gear(JSON::Dict const &json): Gear() {
		/*Constructor from JSON object*/
		if (ISSTR(json.get("name"))) _name = STR(json.get("name")).value();
		if (ISSTR(json.get("description"))) _description = STR(json.get("description")).value();
		if (ISINT(json.get("price"))) _price = INT(json.get("price")).value();		
	}
	operator JSON::Dict() const {
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
protected://modif
    string  _name;
    string  _description;
	int  _price;
};


/*============================================BAT=====================================================*/
class Bat : public Gear 
{
public:
	Bat() : Gear(), _strengthBonus(5), _precisionBonus(5){}
    Bat(const int strength, const int precision) : 	_strengthBonus(strength),
    												_precisionBonus(precision){}
    Bat(const Bat &bat): Gear(bat._name, bat._description, bat._price),
    _strengthBonus(bat._strengthBonus), _precisionBonus(bat._precisionBonus){}//modif
    
    Bat(JSON::Dict const &json) : Gear(json) {
		if (ISINT(json.get("strengthBonus"))) _strengthBonus = INT(json.get("strengthBonus")).value();		
		if (ISINT(json.get("precisionBonus"))) _precisionBonus = INT(json.get("precisionBonus")).value();		
	}

	operator JSON::Dict() const{
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


/*============================================BROOMSTICK==========================================*/
class Broomstick : public Gear 
{
public:
	Broomstick() : Gear(), _velocityBonus(5) {}
	Broomstick(const int velocity) : Gear(), _velocityBonus(velocity) {}
	Broomstick(const Broomstick & broomstick): Gear(broomstick._name,broomstick._description,broomstick._price), 
	_velocityBonus(broomstick._velocityBonus) {} 

	Broomstick(JSON::Dict const &json): Gear(json), _velocityBonus(5) {	
		if (ISINT(json.get("velocityBonus"))) _velocityBonus = INT(json.get("velocityBonus"));		
	}

	operator JSON::Dict() const{
		JSON::Dict res = Gear::operator JSON::Dict();
		res.set("velocityBonus", _velocityBonus);
		return res;
	}

	void setVelocityBonus(const int velocity) { _velocityBonus = velocity; }
	
	int getVelocityBonus () const  	{ return _velocityBonus; }
private:
	int _velocityBonus;
};


/*============================================JERSEY============================================*/
class Jersey : public Gear 
{
public:
	Jersey() : Gear(), _strengthBonus(5) {}
	Jersey(JSON::Dict const &json): Gear(json),
		_strengthBonus(0) {
		if (ISINT(json.get("strengthBonus"))) _strengthBonus = INT(json.get("strengthBonus")).value();		
	}

	Jersey(const Jersey & jersey): Gear(jersey._name,jersey._description,jersey._price),
	_strengthBonus(jersey._strengthBonus){}

	operator JSON::Dict() const {
		JSON::Dict res = Gear::operator JSON::Dict();
		res.set("strengthBonus", _strengthBonus);
		return res;
	}

	int getStrengthBonus () const 			{ return _strengthBonus; }
	void setStrengthBonus(int bonus)		{ _strengthBonus=bonus; }

private:
	int _strengthBonus;

};

#endif
