#include "json.hpp"
#include <sstream>
#include <cstdlib>

extern "C" {
	#include <unistd.h>
	#include <sys/types.h>
	#include <sys/file.h>
	#include <fcntl.h>
	#include <errno.h>
	#include <string.h>
}

#include <string>
using namespace JSON;

#define BUFSIZE 4096

Value *JSON::load(const char *filename){
	/*Method used for reading from <<filename>>*/
	int fd, r;
	char buffer[BUFSIZE+1];
	std::stringstream str;

	fd = open(filename, O_RDONLY);
	if (fd < 0){
		std::string error = strerror(errno);
		error += "in : ";
		error += filename;
		throw IOError(error.c_str());
	}

	/* Acquire an non-exclusive lock on this file */
	int lock = flock(fd, LOCK_SH);
	if (lock != 0){
		std::string error("Couldn't lock file ");
		error += filename;
		close(fd);
		throw IOError(error.c_str());
	}

	while ((r = read(fd, buffer, BUFSIZE)) > 0){
		buffer[r] = '\0';
		str << buffer;
	}
	if (r < 0){
		std::string error = strerror(errno);
		error += "in : ";
		error += filename;
		flock(fd, LOCK_UN);
		close(fd);
		throw IOError(error.c_str());
	}

	flock(fd, LOCK_UN);
	close(fd);
	return parse(str.str().c_str());
}

Value *JSON::load(std::string filename){
	/*Method used for reading from <<filename>>*/
	return JSON::load(filename.c_str());
}

