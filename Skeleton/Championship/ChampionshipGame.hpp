#ifndef ChampionshipGame_hpp
#define ChampionshipGame_hpp

#include "ParticipatingTeam.hpp"
#include "Date.hpp"

class ChampionshipGame 
{
public:
	ParticipatingTeam getTeam1 ();
	ParticipatingTeam getTeam2 ();
	Date getDate ();
    int getTeam1Score();
    int getTeam2Score();
private:
	ParticipatingTeam team1;
	ParticipatingTeam team2;
	Date date;
	int team1Score;
	int team2Score;
};

#endif
