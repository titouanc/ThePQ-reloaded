#include <AdminClient.hpp>
#include <unistd.h>
#include <cstring>

int main(int argc, char const *argv[])
{
	bool has_chdir = false;
	/* Allow relative path for config before chdir */
	NetConfig config;
	if (argc > 1)
		config.load(argv[1]);
	
	/* Standard config path assume we are in client directory */
	if (argc == 1)
		config.load();
	
	AdminClient admin(config);
	if (! has_chdir)
		config.save();
	
	admin.run();
	
	return 0;
}
