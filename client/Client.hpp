#ifndef __CLIENT_HPP
#define __CLIENT_HPP

#include <iostream>
#include <string>
#include <typeinfo>
#include <cxxabi.h>
#include "Message.hpp"
#include "Connection.hpp"
#include "Exception.hpp"
#include "Menu.hpp"
#include "FriendlyGameManager.hpp"
#include <Config.hpp>

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
	std::string _userChoice;
	std::string _prompt;
	Connection _connection;
	
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
	void listUsers();
	void chooseUser();
	
	// utils
	std::string askForUserData(std::string prompt); // returns the user input.
	std::string askForNewPassword(); // prompts the user to create a new password with 
							// confirmation. returns the password.
							
	// Installations
public:
	void printInstallationsList();
	
	void upgradeInstallation();
	void downgradeInstallation();

private:
	void loadInstallations();
	
	std::vector<Installation> _installations;
};

#endif // __CLIENT_HPP
