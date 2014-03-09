#include "user/GraphicUserManager.hpp"
#include "UserData.hpp"
#include <network/ClientConnectionManager.hpp>
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

class GraphicClient {
public:
    GraphicClient(NetConfig const &config);
    void run();
    
private:
    UserData _user;
    net::ClientConnectionManager _connection;
    std::queue<JSON::Dict> _notifications;
    GUI::MainController _controller;
    GraphicUserManager _gUserManager;
};