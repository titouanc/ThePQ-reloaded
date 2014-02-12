#include "json.hpp"
#include <sstream>
#include <cstdlib>

extern "C" {
	#include <unistd.h>
	#include <sys/types.h>
	#include <fcntl.h>
	#include <errno.h>
	#include <string.h>
}

using namespace JSON;

#define BUFSIZE 4096

Value *JSON::load(const char *filename)
{
	int fd, r;
	char buffer[BUFSIZE+1];
	std::stringstream str;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		throw IOError(strerror(errno));

	while ((r = read(fd, buffer, BUFSIZE)) > 0){
		buffer[r] = '\0';
		str << buffer;
	}
	if (r < 0)
		throw IOError(strerror(errno));

	close(fd);
	return parse(str.str().c_str());
}

