#ifndef __SERVER_HPP
#define __SERVER_HPP
#include <iostream>
#include <string>
#include <map>
#include <deque>
#include <Config.hpp>
#include <network/ConnectionManager.hpp>
#include <model/User.hpp>
#include <Config.hpp>
#include "MatchManager.hpp"
#include "PlayerMarket.hpp"
#include "AdminManager.hpp"
#include <model/Championship.hpp>
#include "ServerManager.hpp"
#include "UserManager.hpp"
#include "StadiumManager.hpp"
#include "TeamManager.hpp"
#include "GameManager.hpp"
#include "ChampionshipManager.hpp"


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
class PlayerMarket;
class Server{
public:
	explicit Server(NetConfig const & config);
    ~Server();
	void run();

    /*! Main incoming messages dispatcher */
	void treatMessage(
        int peer_id,
        std::string const & type, 
        const JSON::Value * data
    );

    /*! Create default user accounts if there is no registered user */
    void initDefaultAccounts();

    /* Add/substract team infos based on int sign (if - : substract, if +, add) */
    void endOfMatchTeamInfosUpdate(std::string username, int money, int fame, int ap);
    string getRandomName();
    void collectFinishedMatches(void);

    void timeLoop();
    void timeUpdateStadium();
    void timeUpdateChampionship();
    void addChampionship(const Championship&);

private:
	SharedQueue<net::Message> _inbox, _outbox;
	map<int, User*> _users;
	net::ConnectionManager _connectionManager;
    std::deque<MatchManager*> _matches;
    ServerManager _serverMgr;
    UserManager _userMgr;
    StadiumManager _stadiumMgr;
    TeamManager _teamMgr;
    PlayerMarket _market;
    GameManager _gameMgr;
    ChampionshipManager _championshipMgr;
    AdminManager _adminManager;
    std::deque<Championship*> _championships;
    std::deque<Schedule> _pendingChampMatches;
    unsigned int _timeTicks;

    pthread_mutex_t _champsMutex; //_championships used by 3 threads
	pthread_t _timeThread;
};

#endif
