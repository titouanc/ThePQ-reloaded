#include <network/ConnectionManager.hpp>
#include <network/SubConnectionManager.hpp>
#include <model/User.hpp>
#include <json/json.hpp>
#include <pthread.h>

using namespace net;
#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "admin"

class AdminManager : public SubConnectionManager{
private:
	SharedQueue<Message> _inbox, _outbox;
	User* _admin;
	pthread_t _admin_thread;
	int _admin_peer_id;

public:
	AdminManager(BaseConnectionManager&);
	JSON::Dict loginAdmin(const JSON::Dict&, int);
	void logoutAdmin();
	bool adminIsLogged();
	void run();
	void treatAdminMessage(const Message&);
	void createChampionship(const JSON::Dict&, int);
	User* load(std::string);
	void makeDefaultAdmin();
	void main_loop();
};

















