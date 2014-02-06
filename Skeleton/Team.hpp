#ifndef Team_hpp
#define Team_hpp

#include "Gear.hpp"
#include "Sponsor.hpp"
#include "Installation.hpp"

class Team 
{
public:
	float getRatio ();
	void giveMatchRes (MatchResult result);
	list<Gear> getAvailableEquipment ();
	list<Gear> equipments;
private:
    std::string name;
	list<Member> members;
	int fame;
	int funds;
	list<Sponsor> sponsors;
	list<Installation> stadium;
	int nbrWin;
	int nbrPlayed;
};

#endif
