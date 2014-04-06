#ifndef __MEMBER_HPP
#define __MEMBER_HPP  

//#include "Team.hpp" TODO check if Team is really necessary.
#include <string>
#include <json/json.hpp>
#include <Constants.hpp>
#include <sys/stat.h>

using namespace std;

class Member
{
public:
	/// Constructor
    Member() : _memberID(-1), _name("Lasty Cotter"), _salary(5000), _price(25000), _owner(""){}
    /// Constructor
    Member(int memberID,string name,int salary,int price,string owner) : _memberID(memberID), _name(name), _salary(salary), 
    _price(price), _owner(owner){}
    /// Copy constructor
    Member(JSON::Dict const& json): Member() {
		if (ISINT(json.get("memberID"))) _memberID = INT(json.get("memberID"));
    	if (ISSTR(json.get("name"))) _name = STR(json.get("name")).value();
    	if (ISINT(json.get("salary"))) _salary = INT(json.get("salary")).value();
    	if (ISINT(json.get("price"))) _price = INT(json.get("price")).value();
    	if (ISSTR(json.get(net::MSG::USERNAME))) _owner =  STR(json.get(net::MSG::USERNAME)).value();
    }

	operator JSON::Dict() const {
		JSON::Dict res;
		res.set("memberID", _memberID);
	    res.set("name", _name);
	    res.set("salary", _salary);
	    res.set("price", _price);
	    res.set(net::MSG::USERNAME, _owner);
	    return res;
	}
    /*========Getters===================*/
    string getName () const	{ return _name; }
	int getSalary () const	{ return _salary; }
	int getPrice () const 	{ return _price; }
	int getMemberID() const { return _memberID; }
	string getOwner() const { return _owner; }

    void setName (string name)  { _name = name;     }
    void setSalary (int salary) { _salary = salary; }
    void setPrice (int price)   { _price = price;   }
	void setOwner(string owner)	{ _owner=owner;     }
	//Team getOwner () 	{ return _owner; } // TODO
	//void setOwner (Team aTeam) { _owner = aTeam; } // TODO
	void setMemberID() { _memberID = Member::getNextMemberID(); }
    void setMemberID(int id) {_memberID = id;}
	
	
protected:
	int _memberID;
    string _name;
	int _salary;
	int _price;
	string _owner;
	
	static int getNextMemberID()
	{
		static int _staticMemberID = -1;
		mkdir("data/global", 0755);
		static const std::string path = "data/global/memberID.json";
		if (_staticMemberID == -1)
		{
			try {
				JSON::Value* tmp = JSON::load(path);
				_staticMemberID = INT(tmp);
				delete tmp;
			} catch (JSON::IOError){
				_staticMemberID = 1;
			}
		}
		++_staticMemberID;
		JSON::Integer i(_staticMemberID);
		i.save(path);
		return _staticMemberID;
	}
	
};
#endif
