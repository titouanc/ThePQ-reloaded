#ifndef Member_hpp
#define Member_hpp  

#include "Team.hpp"

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
