#ifndef __SQUAD_HPP
#define __SQUAD_HPP

#include "Constants.hpp"
#include "Player.hpp"

class Squad {
public:
	Squad();
	Squad(string name, Player* players[PLAYERS_NBR_IN_SQUAD]);
	~Squad();

	string getName();
	void setName(string name);

	Player *getPlayers();
	void setPlayers(Player* players[PLAYERS_NBR_IN_SQUAD]);
private:
	string _name;
	Player _players[PLAYERS_NBR_IN_SQUAD];
};

#endif
