#ifndef __CLIENT_MATCH_MANAGER_HPP
#define __CLIENT_MATCH_MANAGER_HPP

#include <model/Pitch.hpp>
#include <model/Displacement.hpp>
#include <model/Position.hpp>
#include <model/Squad.hpp>
#include <network/ConnectionManager.hpp>
#include <Constants.hpp>
#include <json/json.hpp>
#include <map>
#include <string>

class ClientMatchManager {
public:
	ClientMatchManager(SharedQueue<net::Message>&outbox, std::string username);

	void initSquads(const JSON::Value* msg);
	void startMatch(const JSON::Value* msg);
	void turnMenu();
	void displayAvailablePlayers();
	void selectPlayer();
	Position parseDirection(string userInput);
private:
	SharedQueue<net::Message> & _outbox;
	bool _isMatchFinished;
	std::string _username;
	Squad _otherSquad;
	Squad _ownSquad;
	Pitch _pitch;
};

#endif