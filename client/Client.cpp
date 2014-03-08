#include "Client.hpp"
#include <pthread.h>

using namespace std;

std::string humanExcName(const char *name)
{
	int status;
	char *str = abi::__cxa_demangle(name, 0, 0, &status);
	std::string res(str);
	free(str);
	return res;
}

Client::Client(NetConfig const &config) : 	_user(), _notifications(), _connection(config.host, config.port),
												_userManager(_connection, _user, _notifications),
												_isRunning(true)
{}

Client::~Client()
{}

void Client::run()
{
	_connection.run();
	_userManager.run();
}