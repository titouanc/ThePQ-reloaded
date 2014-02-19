#ifndef __SERVER_HPP
#define __SERVER_HPP

#include <iostream>
#include <string>
#include <map>
#include <deque>
#include <Config.hpp>
#include <network/ConnectionManager.hpp>
#include "User.hpp"
#include "MatchManager.hpp"

using namespace std;

struct NetConfig : public Config {
    std::string ip;
    unsigned short port;
    int maxClients;

    NetConfig() : Config("netconfig.json"), ip("127.0.0.1"), port(32123), maxClients(25){}
    void fromDict(JSON::Dict const & json){
        if (ISSTR(json.get("ip")))
            ip = STR(json.get("ip")).value();
        if (ISINT(json.get("port")))
            port = INT(json.get("port"));
        if (ISINT(json.get("maxClients")))
            maxClients = INT(json.get("maxClients"));
    }
    operator JSON::Dict() const{
        JSON::Dict res;
        res.set("ip", ip);
        res.set("port", port);
        res.set("maxClients", maxClients);
        return res;
    }
};

class Server{
public:
	explicit Server(NetConfig const & config);
    ~Server();
	void run();
	void treatMessage(const net::Message &message);

	void registerUser(const JSON::Dict &credentials, int peer_id);
	void logUserIn(const JSON::Dict &credentials, int peer_id);
	void checkIfUserExists(string username, int peer_id);
	void sendInstallationsList(int peer_id);
	void upgradeInstallation(int peer_id, size_t i);
	void downgradeInstallation(int peer_id, size_t i);
	void sendConnectedUsersList(int peer_id);
    string getRandomName();
    void startMatch(int client_idA, int client_idB);
private:
	SharedQueue<net::Message> _inbox, _outbox;
	map<int, User*> _users;
	net::ConnectionManager _connectionManager;
    std::deque<MatchManager*> _matches;
};

#endif
