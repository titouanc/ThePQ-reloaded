#include "Client.hpp"

int main(int argc, char const *argv[])
{
	NetConfig config;
	if (argc > 1)
		config.load(argv[1]);
	else
		config.load();
	

	config.load();
	
	Client client(config);
	config.save();
	
	client.run();
	
	return 0;
}
