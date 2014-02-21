#ifndef __CLIENT_HPP
#define __CLIENT_HPP

#include <iostream>
#include <string>
#include <typeinfo>
#include <cxxabi.h>
#include "Message.hpp"
#include "Client.hpp"
#include "Exception.hpp"
#include "Menu.hpp"
#include <Config.hpp>
#include <model/Player.hpp>
#include <network/TcpSocket.hpp>
#include <stack>
#include "ClientMatchManager.hpp"


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

class CLI
{
public:
	CLI(NetConfig const &config);
	~CLI();
	void run();

private:
	net::TcpSocket _connection;

	std::string _userChoice;
	std::string _prompt;
	
	std::vector<Installation> _installations;
	std::vector<Sale> _playersOnSale;//modif
	std::vector<Player> _players;
	
	std::string _username;
	int _team_id;
	
	pthread_t _thread;
	
	JSON::Value* waitForMsg(std::string);
	std::stack<JSON::Value*> _messages;

	// NEW
	void loginUser(std::string username, std::string passwd);
	void doesUserExist(std::string username);
	void registerUser(std::string username, std::string passwd);
	
	std::vector<Installation> getInstallationsList();
	bool upgradeInstallation(size_t i);
	bool downgradeInstallation(size_t i);
	std::vector<std::string> getConnectedUsersList();

	std::vector<Sale> updatePlayersOnSale();
	void bidOnPlayer(int player_id, std::string username, int value);//modif
	void addPlayerOnMarket(int player_id, std::string username, int value);//modif
	std::vector<Player> getPlayers(std::string username);//modif
	// END NEW
	
	// User menu
	void registerUser();
	void login();
	
	// Menus
	void loginMenu();
	void mainMenu();
	void managementMenu();
	void stadiumMenu();
	void playersMenu();
	void friendlyMatchMenu();
	void marketMenu();
	// utils
	std::string askForUserData(std::string prompt); // returns the user input.
	std::string askForNewPassword(); // prompts the user to create a new password with 
							// confirmation. returns the password.
							
	// Installations
public:
	void printInstallationsList();
	
	void upgradeInstallation();
	void downgradeInstallation();

	// Player market
	void printPlayersOnSale();
	void printPlayers();
	void salePlayer();
	vector<int> getBidValueRange(Player *player);
	void placeBid();

private:
	void loadInstallations();
	
	// Match
	void printConnectedUsersList();
	void chooseUser();
};

#endif // __CLIENT_HPP
