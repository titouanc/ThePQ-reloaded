#include <string>
#include "../cli/Menu.hpp"
#include "../UserData.hpp"
#include <Config.hpp>
#include <iostream>
#include <network/ClientConnectionManager.hpp>
#include <Exception.hpp>


struct NetConfig : public Config {
    std::string host;
    unsigned short port;
    ///constructor
    NetConfig() : Config("netconfig.json"), host("127.0.0.1"), port(32123){}
    void fromDict(JSON::Dict const & json){
        if (ISSTR(json.get("host")))
            host = STR(json.get("host")).value();
        if (ISINT(json.get("port")))
            port = INT(json.get("port"));
    }
    ///operator overloading
    operator JSON::Dict() const{
        JSON::Dict res;
        res.set("host", host);
        res.set("port", port);
        return res;
    }
};


class AdminClient {
private:
	net::ClientConnectionManager _connection;
	bool _isRunning;
	UserData _admin;

public:
    ///constructor
	AdminClient(const NetConfig&);
	void run();
	void showAdminMenu();
	bool showBaseMenu();
	void showIdentifyMenu();
	void showCreateChampionshipMenu();

	void loginAdmin(std::string,std::string); 
	void logoutAdmin();

    void createChampionship(std::string, int, int, int);
};

