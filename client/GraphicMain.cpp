#include "GraphicClient.hpp"
#include <unistd.h>
#include <cstring>

int main(int argc, char const *argv[])
{
	bool has_chdir = false;
	/* Allow relative path for config before chdir */
	NetConfig config;
	if (argc > 1)
		config.load(argv[1]);

	const char *slash = strrchr(argv[0], '/');
	if (slash){
		/* If there are slash in call path, cd into client directory first */
		std::string clientdir(argv[0], slash-argv[0]);
		chdir(clientdir.c_str());
		cout << "CHDIR " << clientdir << endl;
		has_chdir = true;
	}
	
	/* Standard config path assume we are in client directory */
	if (argc == 1)
		config.load();
	
	GraphicClient gClient(config);
	if (! has_chdir)
		config.save();

	gClient.run();
	
	return 0;
}
