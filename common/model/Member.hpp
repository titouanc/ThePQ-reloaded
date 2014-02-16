#ifndef __MEMBER_HPP
#define __MEMBER_HPP  

#include "Team.hpp"
#include <string>

using namespace std;

class Member 
{
public:
	// TODO set random name in constructor
    Member() : name("Lasty Cotter"), salary(5000), price(25000), owner(new Team()){}
    string getName () 	{ return _name; }
	int getSalary () 	{ return _salary; }
	int getPrice () 	{ return _price; }
	Team getOwner () 	{ return _owner; }
	void setOwner (Team aTeam) { _owner = aTeam; }
private:
    string _name;
	int _salary;
	int _price;
	Team * _owner;
};

#endif
