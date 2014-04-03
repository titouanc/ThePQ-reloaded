#include "parser.hpp"
#include "error.hpp"
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
#define BUFSIZE 4096

using namespace JSON;

Value *JSON::readFD(int fd){
	char buffer[BUFSIZE];
	int r = 0;
	std::stringstream str;
	while ((r = read(fd, buffer, BUFSIZE)) > 0){
		buffer[r] = '\0';
		str << buffer;
	}
	if (r < 0){
		std::string error = strerror(errno);
		throw IOError(error.c_str());
	}

	return parse(str.str().c_str());
}

void JSON::writeFD(int fd, JSON::Value const & json)
{
	std::string repr = json.dumps();
	int r = 0;
	for (size_t i=0; i<repr.length(); i+=r){
		r = write(fd, repr.c_str()+i, repr.length()-i);
		if (r < 0)
			throw IOError(strerror(errno));
	}
}

/**
 * Method loading info from file
 * @param char : name of the file from whom to load data
 * @return Value : data loaded from the file
 */
Value *JSON::load(const char *filename){
	int fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0){
		std::string error = strerror(errno);
		error += " in ";
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

	Value *res = NULL;
	try{
		res = readFD(fd);
	} catch (IOError & err){
		flock(fd, LOCK_UN);
		close(fd);
		throw IOError(std::string(err.what())+filename);
	} catch (Error & err){
		flock(fd, LOCK_UN);
		close(fd);
		throw err;
	}
	
	flock(fd, LOCK_UN);
	close(fd);
	return res;
}

/**
 * Method loading info from file
 * @param char : name of the file from whom to load data
 * @return Value : data loaded from the file
 */
Value *JSON::load(std::string filename){
	return JSON::load(filename.c_str());
}

