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
	/* out << "\"" << _content << "\"";*/
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

std::string const & String::value(void) const 
{
	return _content;
}
