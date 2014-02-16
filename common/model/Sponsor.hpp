#ifndef Sponsor_hpp
#define Sponsor_hpp

#include "Team.hpp"

class Sponsor 
{
public:
	Sponsor ();
	bool matchConditions (Team team);
private:
	string name;
	int funds;
	int minFame;
};

#endif
