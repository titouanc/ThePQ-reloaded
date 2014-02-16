#ifndef __FOOD_HPP
#define __FOOD_HPP

#include <string>

using namespace std;

class Food 
{
public:
	Food(string name, int price): _name(name), _price(price){}
    string getName () const 	{ return _name; }
	int getPrice () const 			{ return _price; }
	void setPrice (int newPrice)	{ _price = newPrice; }
private:
    const string _name;
	int _price;
};

#endif
