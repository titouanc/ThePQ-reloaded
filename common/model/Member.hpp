#ifndef __MEMBER_HPP
#define __MEMBER_HPP  

//#include "Team.hpp" TODO check if Team is really necessary.
#include <string>
#include <json/json.hpp>

using namespace std;

class Member 
{
public:
	// TODO set random name in constructor
    Member() : _name("Lasty Cotter"), _salary(5000), _price(25000){}
    Member(JSON::Dict const& json): Member() {
    	if (ISSTR(json.get("name"))) _name = STR(json.get("name")).value();
    	if (ISINT(json.get("salary"))) _salary = INT(json.get("salary")).value();
    	if (ISINT(json.get("price"))) _price = INT(json.get("price")).value();
    }
    operator JSON::Dict(){
		JSON::Dict res;
		res.set("name", _name);
		res.set("salary", _salary);
		res.set("price", _price);
		return res;  
    }
    string getName () 	{ return _name; }
	int getSalary () 	{ return _salary; }
	int getPrice () 	{ return _price; }
	//Team getOwner () 	{ return _owner; } // TODO
	//void setOwner (Team aTeam) { _owner = aTeam; } // TODO
protected:
    string _name;
	int _salary;
	int _price;
	//Team * _owner; // TODO
};

#endif
