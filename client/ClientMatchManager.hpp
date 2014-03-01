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
#include "UserData.hpp"
#include <Exception.hpp>

class ClientMatchManager {
public:
	ClientMatchManager(net::ClientConnectionManager &connection, UserData &user);

	void initBalls(const JSON::Dict& msg);
	void initSquads(const JSON::Dict& msg);
	bool isOwnPlayer(Player const & player);
	void updatePitchWithDeltas();
	void sendStroke(int player, Displacement& currentDisplacement);
	void acceptInvitationFromUser(string username);
	void denyInvitationFromUser(string username);
	std::vector<std::string> getConnectedUsersList();
	void startMatch();
	void chooseUser(std::string);
	void waitForUser();
	
	Squad const & getOwnSquad() { return _ownSquad; }
	Pitch const & getPitch() { return _pitch; }
	bool isMatchFinished() { return _isMatchFinished; }
private:
	net::ClientConnectionManager & _connection;
	UserData &_user;
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
