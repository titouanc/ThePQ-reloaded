#ifndef __TEAM_HPP
#define __TEAM_HPP

#include "Gear.hpp"
#include "Sponsor.hpp"
#include "Installation.hpp"

class Team 
{
public:
	float getRatio () const;
	void giveMatchRes (MatchResult result);
	list<Gear> getAvailableEquipment () const;
	list<Gear> equipments const;
private:
    std::string name;
	list<Member> members;
	int fame;
	int funds;
	list<Sponsor> sponsors;
	list<Installation> stadium;
	float nbrWin;
	float nbrPlayed;
};

#endif
