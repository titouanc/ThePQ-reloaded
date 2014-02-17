#include "Server.hpp"

int main(int argc, char const *argv[])
{
	NetConfig config;
	if (argc > 1)
		config.load(argv[1]);
	else
		config.load();

	Server serv(config);
	config.save(); /* Create config file if inexistant */
	
	serv.run();

	return 0;
}