#include "json.h"
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

Value *JSON::loadFile(const char *filename)
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

void JSON::saveFile(const char *filename, Value const & json)
{
	int fd, r=0;

	fd = open(filename, O_WRONLY);
	if (fd < 0)
		throw IOError(strerror(errno));

	std::string repr = json.dumps();
	for (size_t i=0; i<repr.length(); i+=r){
		r = write(fd, repr.c_str()+i, repr.length()-i);
		if (r < 0)
			throw IOError(strerror(errno));
	}

	close(fd);
}
