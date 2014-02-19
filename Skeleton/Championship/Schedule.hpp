#ifndef Schedule_hpp
#define Schedule_hpp

#include "ChampionshipGame.hpp"

class Schedule 
{
public:
	Schedule();
	ChampionshipGame[] getGames ();
	~Schedule();
private:
	ChampionshipGame[] games;
};

#endif
