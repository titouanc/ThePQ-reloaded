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
	void treatMessage(const net::Message &message);

	void registerUser(const JSON::Dict &credentials, int peer_id);
	User *logUserIn(const JSON::Dict &credentials, int peer_id);
    void checkTeamName(const JSON::Dict &data, int peer_id);
    void sendTeamInfos(const JSON::Dict &data, int peer_id);
	void checkIfUserExists(string username, int peer_id);
	void sendInstallationsList(int peer_id);
	void upgradeInstallation(int peer_id, size_t i);
	void downgradeInstallation(int peer_id, size_t i);
	void sendConnectedUsersList(int peer_id);
    void sendInvitationToPlayer(string const& username, int peer_id);
    void sendInvitationResponseToPlayer(const JSON::Dict &response, int peer_id);
    void sendPlayersOnMarketList(int peer_id);
    void addPlayerOnMarket(const JSON::Dict &bid, int peer_id);
    void placeBidOnPlayer(const JSON::Dict &bid, int peer_id);
    string getRandomName();
    void collectFinishedMatches(void);
    void startMatch(int client_idA, int client_idB, bool champMatch);
    void sendPlayersList(int peer_id);
    void sendMarketMessage(const std::string&, const JSON::Dict&);
    void sendNotification(std::string, const JSON::Dict&);
    int getPeerID(const std::string&);
    void timeLoop();
    void timeUpdateStadium();
    void timeUpdateChampionship();
    void addChampionship(const Championship&);
        /* Return a pointer to the user object with given username,
       or NULL if not connected */
    User *getUserByName(std::string username);
    Championship* getChampionshipByName(std::string champName);
    Championship* getChampionshipByUsername(std::string username);
    size_t nbrUsersConnected(){return _users.size();}
    void loadChampionships();
    void leaveChampionship(int);
    void joinChampionship(std::string,int);
    void sendChampionshipsList(int);
    void sendJoinedChampionship(int);
    void notifyPendingChampMatch(std::string);
    void notifyStartingChampionship(Championship&);
    void responsePendingChampMatch(std::string,int);
    void resolveUnplayedChampMatch(Schedule&);
    void endOfPending(Schedule&);
    Schedule* getPendingMatchByUsername(std::string);
private:
	SharedQueue<net::Message> _inbox, _outbox;
	map<int, User*> _users;
	net::ConnectionManager _connectionManager;
    PlayerMarket *_market;
    std::deque<MatchManager*> _matches;
    AdminManager _adminManager;
    std::deque<Championship*> _championships;
    std::deque<Schedule> _pendingChampMatches;

    pthread_mutex_t _champsMutex; //_championships used by 3 threads
	pthread_t _timeThread;
};

#endif
