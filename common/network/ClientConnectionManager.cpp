#include "ClientConnectionManager.hpp"
#include <cstring>
#include <sstream>

/* Buffer size for incoming messages */
static const size_t MSG_SIZE = 4096;

static void* runClientThread(void* arg)
{
	net::ClientConnectionManager* connection = (net::ClientConnectionManager*)arg;
	connection->start();
	pthread_exit(NULL);
}

void net::ClientConnectionManager::start()
{
	int lock = pthread_mutex_lock(&_mutex);
	if (lock == 0)
	{
		_isRunning = true;
		pthread_mutex_unlock(&_mutex);
	}
	loop();
}

net::ClientConnectionManager::ClientConnectionManager(const std::string hostname, int portno) : 
	_isRunning(false), _sockfd(-1), _hostName(hostname), _port(portno)
{
	pthread_mutex_init(&_mutex, NULL);
}

net::ClientConnectionManager::~ClientConnectionManager()
{
	int lock = pthread_mutex_lock(&_mutex);
	if (lock == 0)
	{
		_isRunning = false;
		pthread_mutex_unlock(&_mutex);
	}
	close(_sockfd);
    pthread_mutex_destroy(&_mutex);
}

void net::ClientConnectionManager::run()
{	
	struct hostent *host = NULL;
    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (_sockfd < 0)
		throw ConnectionFailedException();
    
    host = gethostbyname(_hostName.c_str());
    if (host == NULL)
		throw ConnectionFailedException();
    
    _servAddr.sin_family = AF_INET;
    _servAddr.sin_addr = *((struct in_addr*) *(host->h_addr_list));
    _servAddr.sin_port = htons(_port);

    if (::connect(_sockfd, (struct sockaddr *) &_servAddr,
		sizeof(_servAddr)) < 0)
	{
		throw ConnectionFailedException();
	}
	pthread_create(&_thread, NULL, runClientThread, this);
}

bool net::ClientConnectionManager::isRunning()
{
	bool res = false;
	int lock = pthread_mutex_lock(&_mutex);
	if (lock == 0)
	{
		res = _isRunning;
		pthread_mutex_unlock(&_mutex);
	}
	return res;
}

#define min(a,b) ((a) < (b)) ? (a) : (b)
void net::ClientConnectionManager::loop()
{
	while (_isRunning)
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
			r = recv(_sockfd, data, min(len, MSG_SIZE), 0);
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

JSON::Value* net::ClientConnectionManager::popMessage()
{
	return _messages.pop();
}

bool net::ClientConnectionManager::hasMessage()
{
	return _messages.available();
}



