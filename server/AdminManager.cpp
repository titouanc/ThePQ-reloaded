#include "AdminManager.hpp"

AdminManager::AdminManager(BaseConnectionManager & connections, int peer_id, User* admin) : 
	SubConnectionManager(_inbox, _outbox, connections), _admin(admin), _admin_thread(), 
	_admin_peer_id(peer_id)
{
	acquireClient(peer_id);
	std::cout << peer_id << "\033[32m Admin connected.\033[0m" << std::endl; 
}

/* Creates an admin account */
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

/* Main thread creation + starting in and out queues */
void AdminManager::run(){
	start();
	if(pthread_create(&_admin_thread, NULL, runAdminThread, this) != 0)
		throw "Error occured when starting main admin thread";
}

static void* runAdminThread(void* p){
	AdminManager *manager = static_cast<AdminManager*>(p);
	manager->main_loop();
	phtread_exit(NULL);
}

void AdminManager::main_loop(){
	while (_parent.isRunning() || _inbox.available()){
		Message const & msg = _inbox.pop();
		treatAdminMessage(msg);
		delete msg.data;
	}
}

void AdminManager::treatAdminMessage{
	if (ISDICT(message.data)){
		JSON::Dict const &received = DICT(message.data);
		if (ISSTR(received.get("type"))) {
			string messageType = STR(received.get("type")).value();
			if(ISDICT(message.get("data"))){
				if(messageType == net::MSG::CHAMPIONSHIP_CREATION){
					createChampionship(DICT(received.get("data")),message.peer_id);
				}
			}
		}
	}
}

void AdminManager::createChampionship(const JSON::Dict& data, int peer_id){
	std::cout<<"\033[32mCREATING CHAMPIONSHIP\033[0m"<<std::endl;
}

void AdminManager::logAdminOut(){
	delete admin;
	admin = NULL;
	releaseClient();
	stop();
}

void AdminManager::adminIsLogged(){
	return admin != NULL;
}