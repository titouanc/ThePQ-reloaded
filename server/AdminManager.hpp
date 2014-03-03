#include <network/ConnectionManager.hpp>
#include <network/SubConnectionManager.hpp>
#include <map>
#include <model/User.hpp>

#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "admin"

class AdminManager : public SubConnectionManager{
private:
	SharedQueue<Message> _inbox, _outbox;
	std::map<int, User> _admins;


public:
	static void load(std::string);
	static void makeDefaultAdmin();
};

AdminManager::AdminManager(BaseConnectionManager & connections, int peer_id) : 
	SubConnectionManager(_inbox, _outbox, connections)
{
	acquireClient(peer_id);
	std::cout << peer_id << "\033[32m Admin connected.\033[0m" << std::endl; 
}


void AdminManager::makeDefaultAdmin(){
	User admin(ADMIN_USERNAME,ADMIN_PASSWORD);
	MemoryAccess::saveAdmin(admin);
}

void AdminManager::load(std::string username){
	User* admin = new User(username);
	try{
		MemoryAccess::loadAdmin(*admin);
		return admin;
	}
	catch(const JSON::IOError& e){
		return NULL;
	}
}	