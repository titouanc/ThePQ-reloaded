#ifndef __CLIENT_MATCH_MANAGER_HPP
#define __CLIENT_MATCH_MANAGER_HPP

#include <model/Pitch.hpp>
#include <model/Displacement.hpp>
#include <model/Position.hpp>
#include <model/Squad.hpp>
#include <model/Ball.hpp>
#include <Constants.hpp>
#include <json/json.hpp>
#include <map>
#include <string>
#include <network/ClientConnectionManager.hpp>
#include "User.hpp"

class ClientMatchManager {
public:
	ClientMatchManager(net::ClientConnectionManager &connection, User &user);

	void initBalls(const JSON::Dict& msg);
	void initSquads(const JSON::Dict& msg);
	void startMatch();
	void turnMenu();
	void displayAvailablePlayers();
	void selectPlayer();
	void selectDirectionForPlayer(int player);
	Position parseDirection(string userInput);
	bool isOwnPlayer(Player const & player);
	char playerLetter(Player const & player);
	std::string colorPlayerLetter(Player const & player);
	void displayPitch();
	void updatePitchWithDeltas(JSON::Dict& deltas);
	void updatePitch();
private:
	net::ClientConnectionManager & _connection;
	User &_user;
	bool _isMatchFinished;
	Squad _otherSquad;
	Squad _ownSquad;
	Pitch _pitch;
	/* Balls */
	Quaffle _quaffle;
	GoldenSnitch _snitch;
	Bludger _bludgers[2];
};

#endif
