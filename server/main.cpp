#include "Server.hpp"
/// Main loop handling the server start
int main(int argc, char const *argv[])
{
	bool has_chdir = false;

	/* Allow relative path for config before chdir */
	NetConfig config;
	if (argc > 1)
		config.load(argv[1]);

	const char *slash = strrchr(argv[0], '/');
	if (slash){
		/* If there are slash in call path, cd into server directory first */
		std::string serverdir(argv[0], slash-argv[0]);
		chdir(serverdir.c_str());
		cout << "CHDIR " << serverdir << endl;
		has_chdir = true;
	}
	
	/* Standard config path assume we are in server directory */
	if (argc == 1)
		config.load();

	Server serv(config);
	if (! has_chdir)
		config.save(); /* Create config file if inexistant */
	
	serv.run();

	return 0;
}