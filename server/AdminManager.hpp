#include <network/ConnectionManager.hpp>
#include <network/SubConnectionManager.hpp>
#include <model/User.hpp>
#include <json/json.hpp>
#include <pthead>

#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "admin"

class AdminManager : public SubConnectionManager{
private:
	SharedQueue<Message> _inbox, _outbox;
	User* _admin;
	pthread_t _admin_thread;
	int _admin_peer_id;

public:
	AdminManager(BaseConnectionManager&, int, User*);
	void run();
	void treatAdminMessage();
	void createChampionship(const JSON::Dict&, int);
	static void load(std::string);
	static void makeDefaultAdmin();
	void main_loop();
	void logAdminOut();
	bool adminIsLogged();
};


















