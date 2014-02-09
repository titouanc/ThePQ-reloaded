#include "json.h"

using namespace JSON;

String::String(std::string str) : _content(str)
{}

String::~String()
{}

Type String::type(void) const 
{
	return String_t;
}

Value * String::clone(void) const 
{
	return new String(value());
}

void String::_writeTo(std::ostream & out) const 
{
	out << "\"" << _content << "\"";
}

std::string const & String::value(void) const 
{
	return _content;
}
