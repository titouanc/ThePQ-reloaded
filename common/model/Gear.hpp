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
	int getPrice () const;				{ return _price; }
private:
    string const _name;
    string const _description;
	int const _price;
};

#endif
