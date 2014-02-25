#ifndef __CLIENT_HPP
#define __CLIENT_HPP

#include <iostream>
#include <string>
#include <typeinfo>
#include <cxxabi.h>
#include "Message.hpp"
#include "Exception.hpp"
#include "Menu.hpp"
#include <Config.hpp>
#include <model/Player.hpp>
#include <network/ClientConnectionManager.hpp>
#include <queue>
#include "ClientMatchManager.hpp"
#include <model/Installation.hpp>
#include <model/Sale.hpp>
#include "UserManager.hpp"
#include "User.hpp"
#include "StadiumManager.hpp"
#include "TeamManager.hpp"


struct NetConfig : public Config {
    std::string host;
    unsigned short port;

    NetConfig() : Config("netconfig.json"), host("127.0.0.1"), port(32123){}
    void fromDict(JSON::Dict const & json){
        if (ISSTR(json.get("host")))
            host = STR(json.get("host")).value();
        if (ISINT(json.get("port")))
            port = INT(json.get("port"));
    }
    operator JSON::Dict() const{
        JSON::Dict res;
        res.set("host", host);
        res.set("port", port);
        return res;
    }
};

class Client
{
public:
	Client(NetConfig const &config);
	~Client();
	void run();

private:
	// Cache
	User _user;
	std::vector<Sale> _playersOnSale;
	
	// Managers
	net::ClientConnectionManager _connection;
	UserManager _userManager;
	StadiumManager _stadiumManager;
	ClientMatchManager _matchManager;
	TeamManager _teamManager;
	
	// CLI
	std::string _prompt;
	bool _isRunning;
	
	// Menus
	void mainMenu();
	void managementMenu();
	void friendlyMatchMenu();
	void marketMenu();
	void notificationsMenu();
	
	// Notifications
	void handleNotification(JSON::Value* notification);

	// Match
	void handleFriendlyGameInvitation(JSON::Dict &message);
	void acceptInvitationFromUser(string username);
	void denyInvitationFromUser(string username);
	std::vector<std::string> getConnectedUsersList();
	void printConnectedUsersList();
	void chooseUser();
	void startMatch();

	// Market
	std::vector<Sale> updatePlayersOnSale();
	void bidOnPlayer(int player_id, std::string username, int value);
	void addPlayerOnMarket(int player_id, std::string username, int value);
	void printPlayersOnSale();
	void salePlayer();
	vector<int> getBidValueRange(Player *player);
	void placeBid();
	void handleEndOfSaleNotification(JSON::Dict&);

	// utils
	std::string askForUserData(std::string prompt); // returns the user input.
	std::string askForNewPassword(); // prompts the user to create a new password with 
							// confirmation. returns the password.

};

#endif // __CLIENT_HPP
