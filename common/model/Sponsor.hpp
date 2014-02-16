#ifndef __SPONSOR_HPP
#define __SPONSOR_HPP

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
