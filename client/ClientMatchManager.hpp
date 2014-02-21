#ifndef __CLIENT_MATCH_MANAGER_HPP
#define __CLIENT_MATCH_MANAGER_HPP

#include <model/Pitch.hpp>
#include <model/Displacement.hpp>
#include <model/Position.hpp>
#include <model/Squad.hpp>
#include <model/Ball.hpp>
#include <network/ConnectionManager.hpp>
#include <Constants.hpp>
#include <json/json.hpp>
#include <map>
#include <string>

class ClientMatchManager {
public:
	ClientMatchManager();

	void initBalls(const JSON::Dict& msg);
	void initSquads(const JSON::Dict& msg, string username);
	void startMatch(const JSON::Value* msg);
	void turnMenu();
	void displayAvailablePlayers();
	void selectPlayer();
	Position parseDirection(string userInput);
private:
	bool _isMatchFinished;
	std::string _username;
	Squad _otherSquad;
	Squad _ownSquad;
	Pitch _pitch;
	/* Balls */
	Quaffle _quaffle;
	GoldenSnitch _snitch;
	Bludger _bludgers[2];
};

#endif