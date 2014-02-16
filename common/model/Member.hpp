#ifndef __MEMBER_HPP
#define __MEMBER_HPP  

//#include "Team.hpp" TODO check if Team is really necessary.
#include <string>

using namespace std;

class Member 
{
public:
	// TODO set random name in constructor
    Member() : _name("Lasty Cotter"), _salary(5000), _price(25000){}
    string getName () 	{ return _name; }
	int getSalary () 	{ return _salary; }
	int getPrice () 	{ return _price; }
	//Team getOwner () 	{ return _owner; } // TODO
	//void setOwner (Team aTeam) { _owner = aTeam; } // TODO
private:
    string _name;
	int _salary;
	int _price;
	//Team * _owner; // TODO
};

#endif
