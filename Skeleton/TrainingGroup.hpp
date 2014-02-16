#ifndef __TRAINING_GROUP_HPP
#define __TRAINING_GROUP_HPP

#include "Coach.hpp"
#include "Player.hpp"

class TrainingGroup 
{
public:
	TrainingGroup ();
	void train ();
	void addCoach (Coach aCoach);
	void removeCoach (Coach aCoach);
	list<Coach> getCoaches ();
	void addPlayer (Player aPlayer);
	void removePlayer (Player aPlayer);
	list<Player> getPlayers ();
private:
	list<Coach> coaches;
	list<Player> players;
};

#endif
