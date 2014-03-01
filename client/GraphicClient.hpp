#include "GraphicUserManager.hpp"
#include "UserData.hpp"
#include <network/ClientConnectionManager.hpp>

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

class GraphicClient {
public:
    GraphicClient(NetConfig const &config) :
    _user(), _connection(config.host, config.port), _controller(), _gUserManager(_connection, _user, _controller){

    }

private:
    UserData _user;
    net::ClientConnectionManager _connection;
    GUI::MainController _controller;
    GraphicUserManager _gUserManager;
};