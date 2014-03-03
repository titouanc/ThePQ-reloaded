#include <string>
#include "../Menu.hpp"
#include "../UserManager.hpp"
#include <iostream>
#include <network/ClientConnectionManager.hpp>
#include <Exception.hpp>


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


class AdminClient{
private:
	net::ClientConnectionManager _connection;
	bool _isRunning;
	UserData _user;
	UserManager _userManager;



public:
	void run();
};


AdminClient::AdminClient(NetConfig const &config) : _connection(config.host, config.port), _isRunning(true),
_user(), _userManager(_connection,_user){}

void AdminClient::run(){
	std::cout<<"Welcome. This is the admin client for The Pro Quidditch Manager 2014."<<std::endl;
	while(_isRunning){
		if(_user.isLogged()){
			showAdminMenu();
		}
		else{
			_isRunning = showBaseMenu();
		}
	}
	std::cout<<"a+"<<std::endl;
}

void AdminClient::showAdminMenu(){
	
}

void AdminClient::showBaseMenu(){
	Menu _menu;
	_menu.addToDisplay("   - identify\n");
	_menu.addToDisplay("   - quit\n");
	int option;
	bool res = true;
	option = _menu.run();
	switch(option)
	{
		case 1:
			showIdentifyMenu();
			break;
		default:
			res = false;
			break;
	}
	return res;
}

void AdminClient::showIdentifyMenu(){	
	std::string username = Menu::askForUserData("Username : ");
	std::string password = Menu::askForUserData("Password : ");
	
	try {
		std::cout << "Please wait..." << std::endl;
		_userManager.loginUser(username, password);
		std::cout << "You have successfully logged in as administrator." << std::endl;
	}
	catch (const AlreadyLoggedInException& e){
		std::cout << "\033[31mError :\033[0m the admin client is already being used." <<std::endl;
	}
	catch (const BaseLogInException& e){
		std::cout << "\033[31mError :\033[0m wrong combination." << std::endl;
	}
}

