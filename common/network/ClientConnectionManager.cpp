#include "ClientConnectionManager.hpp"
#include <cstring>
#include <sstream>

void net::ClientConnectionManager::send(JSON::Value const & json)
{
	std::string dump = json.dumps();
	const char* data = dump.c_str();
	uint32_t len = htonl(dump.length());
	int r = ::send (_sockfd, &len, 4, 0);
	if (r != 4)
		throw DisconnectedException();
	
	for (size_t i=0; i<dump.length(); i+=r){
		r = ::send(_sockfd, data+i, dump.length()-i, 0);
		if (r <= 0){
			throw DisconnectedException();
		}
	}
}

JSON::Value* net::ClientConnectionManager::waitForMsg(std::string typeToWait)
{
	_isWaitingForMessage = true;
	JSON::Value* msg = NULL, *res = NULL;
	while (res == NULL || _messages.available())
	{
		msg = _messages.pop();
		JSON::Dict const & dict = DICT(msg);
		if (STR(dict.get("type")).value() == typeToWait)
		{
			res = msg;
		}
		else
		{
			notifications.push(msg);
		}
	}
	_isWaitingForMessage = false;
	return res;
}
JSON::Value* net::ClientConnectionManager::hasMessageTypeInNotifications(std::string messageType){
	JSON::Value* res = NULL;
	for (size_t i = 0; i<notifications.size(); ++i){
		JSON::Dict notif = DICT(notifications.front());
		if (ISSTR(notif.get("type"))&& STR(notif.get("type")).value() == messageType){
			res =  notifications.front();
			notifications.pop();
		}
		notifications.push(notifications.front());
		notifications.pop();
	}
	return res;
}

void net::ClientConnectionManager::updateNotifications(){
	while (!_isWaitingForMessage && _messages.available()){
		notifications.push(_messages.pop());
	}
}

void net::ClientConnectionManager::loop()
{
	while (_isRunning == true)
	{
		std::stringstream res;
		char data[MSG_SIZE+1];
		bzero(data, MSG_SIZE);
		uint32_t len = 0;
		int r = ::recv(_sockfd, &len, 4, 0);
		len = ntohl(len);
		if (r != 4)
		{
			throw DisconnectedException();
		}
		
		
		while (len > 0) {
			r = recv(_sockfd, data, MSG_SIZE, 0);
			if (r == 0) throw DisconnectedException();
			data[r] = '\0';
			res << data;
			len -= r;
		}
		JSON::Value* json = JSON::parse(res.str().c_str());
		if (json != NULL && ISDICT(json))
		{
			JSON::Dict const & dict = DICT(json);
			if (dict.hasKey("type") && dict.hasKey("data") && ISSTR(dict.get("type")))
			{
				_messages.push(json);
			}
		}
	}
}

// TODO Mutex on running ?

void net::ClientConnectionManager::start()
{
	_isRunning = true;
	loop();
}

net::ClientConnectionManager::ClientConnectionManager(const std::string hostname, int portno) : 
	_isRunning(false), _isWaitingForMessage(false), _sockfd(-1)
{
	struct hostent *host = NULL;
    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (_sockfd < 0)
		throw ConnectionFailedException();
    
    host = gethostbyname(hostname.c_str());
    if (host == NULL)
		throw ConnectionFailedException();
    
    _servAddr.sin_family = AF_INET;
    _servAddr.sin_addr = *((struct in_addr*) *(host->h_addr_list));
    _servAddr.sin_port = htons(portno);

    if (::connect(_sockfd, (struct sockaddr *) &_servAddr,
		sizeof(_servAddr)) < 0)
	{
		throw ConnectionFailedException();
	}
	pthread_create(&_thread, NULL, net::runClientThread, this);
}

net::ClientConnectionManager::~ClientConnectionManager()
{
	_isRunning = false;
	::close(_sockfd);
}
