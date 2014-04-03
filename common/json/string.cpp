#include "json.hpp"

using namespace JSON;

String::String(std::string str) : _content(str)
{/*Constructor for String object*/}

String::~String()//Destructor for String object
{}

/**
 * Method returning the type of the object
 */
Type String::type(void) const 
{
	return String_t;
}

/**
 * Method returning a pointer to the content of object String
 */
Value * String::clone(void) const 
{
	
	return new String(value());
}

/**
 * Method for putting <<out>> to the ofstream buffer
 * eg buff: "\ _content \" out<<buff
 */
void String::_writeTo(std::ostream & out) const 
{
	
	out << "\"";
	for (size_t i=0; i<_content.length(); i++){
		switch (_content[i]){
			case '\\': out << "\\\\"; break;
			case '\a': out << "\\a"; break;
			case '\b': out << "\\b"; break;
			case '\n': out << "\\n"; break;
			case '\r': out << "\\r"; break;
			case '\t': out << "\\t"; break;
			case '"': out << "\\\""; break;
			default:
				if (_content[i] <= 0x1f || _content[i] >= 0x7f)
					out << "\\x" << std::hex << (int) _content[i] << std::dec;
				else
					out << _content[i];
		}
	}
	out << "\"";
}

/**
 * Method returning a const reference of the value of String
 */
std::string const & String::value(void) const 
{
	return _content;
}
