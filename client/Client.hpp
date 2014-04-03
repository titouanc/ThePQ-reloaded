#ifndef __CLIENT_HPP
#define __CLIENT_HPP


#include <iostream>
#include <string>
#include <typeinfo>
#include <cxxabi.h>
#include <Exception.hpp>
#include "cli/Menu.hpp"
#include <Config.hpp>
#include <network/ClientConnectionManager.hpp>
#include <queue>
#include "user/CLIUserView.hpp"
#include "UserData.hpp"
#include <json/json.hpp>

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
	std::queue<JSON::Dict> _notifications;
	
	// Managers
	net::ClientConnectionManager _connection;
	CLIUserView _userManager;
	
	// CLI
	bool _isRunning;
};

#endif // __CLIENT_HPP
