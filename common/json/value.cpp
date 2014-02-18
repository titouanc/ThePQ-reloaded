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
	/*Method for dumping to the ofstream*/
	std::stringstream res;
	this->_writeTo(res);
	return res.str();
}

void Value::save(const char *filename) const
{
	/*Method for saving to <<filename>>*/
	int fd, r=0;

	fd = open(filename, O_WRONLY|O_CREAT|O_TRUNC, 0644);
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
	/*Method for saving a dump to <<filename>>*/
	save(filename.c_str());
}
