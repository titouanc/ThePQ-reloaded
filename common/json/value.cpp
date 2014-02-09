#include "json.h"
#include <sstream>

using namespace JSON;

std::string Value::dumps(void) const
{
	std::stringstream res;
	this->_writeTo(res);
	return res.str();
}
