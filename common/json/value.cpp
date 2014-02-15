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

std::string Value::dumps(void) const
{
	std::stringstream res;
	this->_writeTo(res);
	return res.str();
}

void Value::save(const char *filename) const
{
	int fd, r=0;

	fd = open(filename, O_WRONLY|O_CREAT, 0644);
	if (fd < 0)
		throw IOError(strerror(errno));

	std::string repr = this->dumps();
	for (size_t i=0; i<repr.length(); i+=r){
		r = write(fd, repr.c_str()+i, repr.length()-i);
		if (r < 0)
			throw IOError(strerror(errno));
	}

	close(fd);
}

void Value::save(std::string const & filename) const 
{
	save(filename.c_str());
}
