#ifndef __CLIENT_HPP
#define __CLIENT_HPP


#include <iostream>
#include <string>
#include <typeinfo>
#include <cxxabi.h>
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
#include "UserData.hpp"
#include "StadiumManager.hpp"
#include "TeamManager.hpp"
#include "MarketManager.hpp"


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
	UserData _user;
	
	// Managers
	net::ClientConnectionManager _connection;
	UserManager _userManager;
	StadiumManager _stadiumManager;
	ClientMatchManager _matchManager;
	TeamManager _teamManager;
	MarketManager _marketManager;
	
	// CLI
	bool _isRunning;
	std::string _prompt;
	std::string splashScreen();
	std::string goodBye();
	
	// Users
	bool showUserMenu();
	void showLoginMenu();
	void showRegisterMenu();
	
	// Team
	void showTeamMenu();
	void printPlayers();
	
	// Market
	void showMarketMenu();
	void placeBid();
	void seePlayersOnSale();
	void printPlayersOnSale();
	void salePlayer();
	
	// Menus
	void mainMenu();
	void managementMenu();
	void friendlyMatchMenu();
	void notificationsMenu();
	
	// Notifications
	void handleNotification(JSON::Value* notification);
	void handleEndOfSaleNotification(JSON::Dict&);
	void handleFriendlyGameInvitation(JSON::Dict &message);
	
	// Match
	void printConnectedUsersList();
	void chooseUserMenu();
	void turnMenu();
	void displayAvailablePlayers();
	void selectPlayer();
	void selectDirectionForPlayer(int player);
	Position parseDirection(string userInput);
	char playerLetter(Player const & player);
	std::string colorPlayerLetter(Player const & player);
	void displayPitch();
};

#endif // __CLIENT_HPP
