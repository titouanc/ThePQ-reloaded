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
	AdminManager(BaseConnectionManager&);
	void loginAdmin(User*, int);
	void logoutAdmin();
	bool adminIsLogged();
	void run();
	void treatAdminMessage();
	void createChampionship(const JSON::Dict&, int);
	void load(std::string);
	static void makeDefaultAdmin();
	void main_loop();
	
	
};


















