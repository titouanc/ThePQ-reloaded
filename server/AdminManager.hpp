#ifndef __ADMINMANAGER_HPP
#define __ADMINMANAGER_HPP
#include <network/ConnectionManager.hpp>
#include <network/SubConnectionManager.hpp>
#include <model/User.hpp>
#include <json/json.hpp>
#include <pthread.h>

class Server;
using namespace net;
#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "admin"

class AdminManager : public SubConnectionManager{
private:
	SharedQueue<Message> _inbox, _outbox;
	Server* _server;
	User* _admin;
	int _admin_peer_id;

public:
	AdminManager(BaseConnectionManager&, Server*);
	void loginAdmin(const JSON::Dict&, int);
	void logoutAdmin();
	bool adminIsLogged();
	void run();
	void treatAdminMessage(const Message&);
	void createChampionship(const JSON::Dict&, int);
	User* load(std::string);
	void makeDefaultAdmin();
	void _mainloop_out();
};

#endif
















