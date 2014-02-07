#include "json.h"
#include <sstream>

using namespace JSON;

Dict::Dict() : _content() 
{}

Dict::~Dict() 
{
    std::map<std::string, Value*>::iterator elem;
    for (elem=_content.begin(); elem!=_content.end(); elem++)
        delete elem->second;
    _content.clear();
}

Type Dict::type(void) const 
{
	return Dict_t;
}

Value * Dict::clone(void) const 
{
	return NULL;
}

std::string Dict::dumps(void) const 
{
    std::stringstream res;
    res << "{";
    std::map<std::string, Value*>::const_iterator elem;
    for (elem=_content.begin(); elem!=_content.end(); elem++){
        if (elem != _content.begin()) res << ", ";
        res << "\"" << elem->first << "\": " 
            << elem->second->dumps();
    }
    res << "}";
    return res.str();
}

bool Dict::hasKey(std::string const & key)
{
    return _content.find(key) != _content.end();
}

void Dict::setPtr(std::string const & key, Value *ptr)
{
    _content.insert(
        _content.begin(), 
        std::pair<std::string, Value*>(key, ptr)
    );
}

void Dict::set(std::string const & key, Value const & val)
{
    setPtr(key, val.clone());
}

const Value * Dict::get(std::string const & key)
{
    std::map<std::string, Value*>::const_iterator elem;
    elem = _content.find(key);
    if (elem == _content.end()) 
        throw KeyError(key);
    return elem->second;
}