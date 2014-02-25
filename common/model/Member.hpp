#ifndef __MEMBER_HPP
#define __MEMBER_HPP  

//#include "Team.hpp" TODO check if Team is really necessary.
#include <string>
#include <json/json.hpp>
#include <Constants.hpp>
#include <sys/stat.h>
#include <model/Savable.hpp>

using namespace std;

class Member : public Savable<Member>
{
public:
	// TODO set random name in constructor-<<<<load first/last name from playerNames.txt
    Member() : _memberID(-1), _name("Lasty Cotter"), _salary(5000), _price(25000){}
    Member(int memberID,string name,int salary,int price) : _memberID(memberID), _name(name), _salary(salary), 
    _price(price){}//modif
    Member(JSON::Dict const& json): Member() {
		if (ISINT(json.get("memberID"))) _memberID = INT(json.get("memberID"));
    	if (ISSTR(json.get("name"))) _name = STR(json.get("name")).value();
    	if (ISINT(json.get("salary"))) _salary = INT(json.get("salary")).value();
    	if (ISINT(json.get("price"))) _price = INT(json.get("price")).value();
    }

	operator JSON::Dict(){
	JSON::Dict res;
	res.set("memberID", _memberID);
    res.set("name", _name);
    res.set("salary", _salary);
    res.set("price", _price);
    return res;
}
    /*========Getters===================*/
    string getName () 	{ return _name; }
	int getSalary () 	{ return _salary; }
	int getPrice () 	{ return _price; }
	int getMemberID()	{ return _memberID; }
	//Team getOwner () 	{ return _owner; } // TODO
	//void setOwner (Team aTeam) { _owner = aTeam; } // TODO
	void setMemberID() { _memberID = Member::getNextMemberID(); }
	
	
protected://modif
	int _memberID;
    string _name;
	int _salary;
	int _price;
	
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
		cout << "MEMBER ID: " << _staticMemberID << endl;
		JSON::Integer i(_staticMemberID);
		i.save(path);
		return _staticMemberID;
	}
	
	//Team * _owner; // TODO
};
#endif
