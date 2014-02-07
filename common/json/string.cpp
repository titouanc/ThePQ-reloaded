#include "json.h"

using namespace JSON;

String::String(std::string str) : _content(str)
{}

Type String::type(void) const 
{
	return String_t;
}

Value * String::clone(void) const 
{
	return new String(value());
}

std::string String::dumps(void) const 
{
	return "\"" + _content + "\"";
}

std::string const & String::value(void) const 
{
	return _content;
}
