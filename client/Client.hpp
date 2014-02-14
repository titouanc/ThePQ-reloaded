#ifndef __CLIENT_HPP
#define __CLIENT_HPP

#include <iostream>
#include <string>
#include "Message.hpp"
#include "Connection.hpp"
#include "Exception.hpp"
#include "Menu.hpp"
#include "StadiumManager.hpp"
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
	static void run();

private:
	Client()
	{}
	static std::string _userChoice;
	static Connection _connection;
	static std::string _prompt;
	
	// User menu
	static void registerUser();
	static void login();
	
	// Menus
	static void mainMenu();
	static void managementMenu();
	static void stadiumMenu();
	static void playersMenu();
	static void friendlyMatchMenu();
	static void listUsers();
	static void chooseUser();
	
	// utils
	static std::string askForUserData(std::string prompt); // returns the user input.
	static std::string askForNewPassword(); // prompts the user to create a new password with 
							// confirmation. returns the password.
};

#endif // __CLIENT_HPP
