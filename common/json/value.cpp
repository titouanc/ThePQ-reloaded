#include "value.hpp"
#include "error.hpp"
#include "parser.hpp"
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

using namespace JSON;

/**
 * Method for dumping to the ofstream
 * @return std::string : string dump
 */
std::string Value::dumps(void) const
{
	std::stringstream res;
	this->_writeTo(res);
	return res.str();
}

/**
 * Method for saving to a file
 * @param char : filename of the file in which the data will be saved
 */
void Value::save(const char *filename) const
{
	
	int fd;

	fd = open(filename, O_WRONLY|O_CREAT|O_TRUNC, 0644);
	if (fd < 0){
		std::string error = strerror(errno);
		error += " in ";
		error += filename;
		throw IOError(error.c_str());
	}

	/* Acquire an exclusive lock on this file */
	int lock = flock(fd, LOCK_EX);
	if (lock != 0){
		std::string error("Couldn't lock file ");
		error += filename;
		close(fd);
		throw IOError(error.c_str());
	}

	try {
		writeFD(fd, *this);
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
}

/**
 * Method for saving a dump to a file
 * @param std::string filename of the file where the dump will be saved
 */
void Value::save(std::string const & filename) const 
{	
	save(filename.c_str());
}
