#include "Server.hpp"

int main(int argc, char const *argv[])
{
	NetConfig config;
	config.load();

	Server serv(config);
	config.save(); /* Create config file if inexistant */
	
	serv.run();

	return 0;
}