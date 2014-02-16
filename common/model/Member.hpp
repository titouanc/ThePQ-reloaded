#ifndef __MEMBER_HPP
#define __MEMBER_HPP  

#include "Team.hpp"
#include <string>

class Member 
{
public:
    Member();
    std::string getName ();
	int getSalary ();
	int getPrice ();
	Team getOwner ();
	void setOwner (Team aTeam);
private:
    std::string name;
	int salary;
	int price;
	Team * owner;
};

#endif
