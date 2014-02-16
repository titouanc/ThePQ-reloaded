#ifndef __SPONSOR_HPP
#define __SPONSOR_HPP

#include <string>

using namespace std;

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
