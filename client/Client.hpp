#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <network/ConnectionManager.hpp>
#include <json/json.hpp>
#include "Exception.hpp"
#include <Constants.hpp>
#include <string>
#include <vector>
#include "model/Installation.hpp"
#include <model/Player.hpp>

class Client
{
public:
	Client(std::string host, int port);
	
	void loginUser(std::string username, std::string passwd);
	void doesUserExist(std::string username);
	void registerUser(std::string username, std::string passwd);
	
	std::vector<Installation> getInstallationsList();
	bool upgradeInstallation(size_t i);
	bool downgradeInstallation(size_t i);
	std::vector<std::string> getConnectedUsersList();

	std::vector<JSON::Dict> updatePlayersOnSale();
	void bidOnPlayer(int player_id, std::string username, int value);//modif
	void addPlayerOnMarket(int player_id, std::string username, int value);//modif
	std::vector<Player> getPlayers(std::string username);//modif


	
private:
	SharedQueue<net::Message> _inbox, _outbox;
	net::ClientConnectionManager _connectionManager;
};

#endif
