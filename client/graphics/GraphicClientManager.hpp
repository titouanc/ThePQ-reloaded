#include "GraphicManager.hpp"
#include "../userData.hpp"
#include <network/ClientConnectionManager.hpp>
using namespace GUI;

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

class GraphicClientManager : public GraphicManager {
public:
    GraphicClientManager(NetConfig const &config, MainController &c) : GraphicManager(c),
    _user(), _connection(config.host, config.port), _userManager(_connection, _user){
        userNameTB = _canvas.addTextBox("Username");
        passWordTB = _canvas.addTextBox("Password");
        _canvas.addButton<GraphicClientManager>(&GraphicClientManager::logIn, this, "Log in");
        displayCanvas();
        run();
    }

private:
    UserData _user;
    net::ClientConnectionManager _connection;
    UserManager _userManager;
    TextBox* userNameTB;
    TextBox* passWordTB;

    void logIn(){
        try{
            _userManager.doLogInUser(userNameTB->getText(),userNameTB->getText());
        }
    }
}