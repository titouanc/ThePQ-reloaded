#ifndef Championship_hpp
#define Championship_hpp

#include "ParticipatingTeam.hpp"
#include "Schedule.hpp"
#include "Ranking.hpp"

class Championship 
{
public:
	Championship();
	ParticipatingTeam[] getTeams ();
	void startChampionship ();
	void addTeam (ParticipatingTeam team);
	void deleteTeam (ParticipatingTeam team);
	Schedule getSchedule (ParticipatingTeam team);
	Ranking getRanking ();
	~Championship();
private:
	ParticipatingTeam[] teams;
	Ranking ranking;
	Schedule schedule;
	int minLevel;
	int maxLevel;
};

#endif
