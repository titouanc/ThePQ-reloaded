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
#include <network/TcpSocket.hpp>

class ClientMatchManager {
public:
	ClientMatchManager(net::TcpSocket &connection);

	void initBalls(const JSON::Dict& msg);
	void initSquads(const JSON::Dict& msg, string username);
	void startMatch();
	void turnMenu();
	void displayAvailablePlayers();
	void selectPlayer();
	void selectDirectionForPlayer(int player);
	Position parseDirection(string userInput);
private:
	net::TcpSocket & _connection;
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