#include "Server.hpp"

int main(int argc, char const *argv[])
{
	NetConfig config;
	config.load();
	config.save(); /* Create config file if inexistant */

	Server s(config);

	return 0;
}