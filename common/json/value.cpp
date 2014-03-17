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

std::string Value::dumps(void) const
{
	/*Method for dumping to the ofstream*/
	std::stringstream res;
	this->_writeTo(res);
	return res.str();
}

void Value::save(const char *filename) const
{
	/*Method for saving to <<filename>>*/
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

void Value::save(std::string const & filename) const 
{
	/*Method for saving a dump to <<filename>>*/
	save(filename.c_str());
}
