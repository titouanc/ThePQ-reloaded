#include "AdminManager.hpp"
#include <unistd.h>
#include "Server.hpp"
AdminManager::AdminManager(BaseConnectionManager & connections, Server* serv) : 
	SubConnectionManager(_inbox, _outbox, connections), _server(serv),_admin(NULL), _admin_peer_id()
{
	makeDefaultAdmin();
}

/* Creates an admin account */
void AdminManager::makeDefaultAdmin(){
	User admin(ADMIN_USERNAME,ADMIN_PASSWORD);
	MemoryAccess::saveAdmin(admin);
}

/* Login handlers */
void AdminManager::loginAdmin(const JSON::Dict& data, int peer_id){
	JSON::Dict response;
	if (ISSTR(data.get(net::MSG::USERNAME)) && ISSTR(data.get(net::MSG::PASSWORD))){
		std::string const & username = STR(data.get(MSG::USERNAME));
		std::string const & password = STR(data.get(MSG::PASSWORD));
		response.set("type", MSG::STATUS);
		if (! adminIsLogged()){
			_admin = load(username);
			if(_admin != NULL){
				if(_admin->getPassword() == password){
					response.set("data",net::MSG::USER_LOGIN);
					_admin_peer_id = peer_id;
					std::cout << "[" << this << "] \033[1m" << peer_id 
					          << "\033[36m  admin connected\033[0m" << std::endl; 
				}
				else{
					response.set("data",net::MSG::PASSWORD_ERROR);
					delete _admin;
					_admin = NULL;
				}
			}
			else
				response.set("data",net::MSG::USER_NOT_FOUND);
		}
		else
			response.set("data",net::MSG::ALREADY_LOGGED_IN);
	}
	_doWrite(peer_id, &response);
}

void AdminManager::logoutAdmin(){
	delete _admin;
	_admin = NULL;
	std::cout << "[" << this << "] \033[1m" << _admin_peer_id 
	          << "\033[36m  admin disconnected\033[0m" << std::endl;
	releaseClient(_admin_peer_id);
	stop();
}

bool AdminManager::adminIsLogged(){
	return _admin != NULL;
}

User* AdminManager::load(std::string username){
	User* admin = new User(username);
	try{
		MemoryAccess::loadAdmin(*admin);
		return admin;
	}
	catch(const JSON::IOError& e){
		return NULL;
	}
}

void AdminManager::_mainloop_out(){
	std::cout << "[" << this << "] \033[31m\033[1mAdmin server start\033[0m" << endl;
	while (isRunning() || _inbox.available()){
		Message const & msg = _inbox.pop();
		if (ISDICT(msg.data))
			treatAdminMessage(msg);
		delete msg.data;
	}
	std::cout << "[" << this << "] \033[31m\033[1mAdmin server stop\033[0m" << endl;
}

/* Admin messages handler */
void AdminManager::treatAdminMessage(const Message &message){
	if (ISDICT(message.data)){
		JSON::Dict const &received = DICT(message.data);
		if (ISSTR(received.get("type"))) {
			string messageType = STR(received.get("type")).value();
			if(messageType == net::MSG::DISCONNECT){
				logoutAdmin();
			}
			if(ISDICT(received.get("data"))){
				if(messageType == net::MSG::ADMIN_LOGIN){
					loginAdmin(DICT(received.get("data")),message.peer_id);
				}
				else if(messageType == net::MSG::CHAMPIONSHIP_CREATION){
					createChampionship(DICT(received.get("data")),message.peer_id);
				}
			}
		}
	}
}

/* Requests */
void AdminManager::createChampionship(const JSON::Dict& data, int peer_id){
	JSON::Dict response;
	if(peer_id != _admin_peer_id){
		response.set("type","WUT HACKER");
		response.set("data","YOU AINT ADMIN WTF");
	}
	else{
		response.set("type",net::MSG::CHAMPIONSHIP_CREATION);
		Championship champ(INT(data.get(net::MSG::TURN_NUMBER)),STR(data.get(net::MSG::CHAMPIONSHIP_NAME)).value(),
			INT(data.get(net::MSG::CHAMPIONSHIP_CASHPRIZE)), INT(data.get(net::MSG::CHAMPIONSHIP_FAME)));
		try{
			MemoryAccess::load(champ);
			response.set("data",net::MSG::CHAMPIONSHIP_ALREADY_EXISTS);
		}
		catch(const JSON::IOError& e){
			response.set("data",net::MSG::CHAMPIONSHIP_CREATED);
			_server->addChampionship(champ);
		}
	}
	_doWrite(peer_id,&response);
}
