#ifndef Ranking_hpp
#define Ranking_hpp

#include "ParticipatingTeam"

class Ranking 
{
private:
	ParticipatingTeam[] teams;
public:
	Ranking();
	ParticipatingTeam[] getRanking ();
	void update ();
	~Ranking();
};

#endif
