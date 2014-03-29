#include "json.hpp"
/** ==Library performing conversion from objects from std library to JSON objects== */
using namespace JSON;

/* ===== Integer ===== */
Type Integer::type(void) const 
{
    return Integer_t;
}

/* ===== Float ===== */
Type Float::type(void) const 
{
    return Float_t;
}

Type Bool::type(void) const
{
	return Boolean_t;
}

void Bool::_writeTo(std::ostream & out) const
{
	if (value())
		out << "true";
	else
		out << "false";
}
