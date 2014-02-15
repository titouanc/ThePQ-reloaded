#include "Client.hpp"

int main(int argc, char const *argv[])
{
	NetConfig config;
	config.load();
	
	Client client(config);
	config.save();
	
	client.run();
	
	return 0;
}
