#include "json.hpp"
#include <cmath>

using namespace JSON;

Dict::Dict() : _content() 
{}

Dict::~Dict() 
{
    std::unordered_map<std::string, Value*>::iterator elem;
    for (elem=_content.begin(); elem!=_content.end(); elem++)
        delete elem->second;
    _content.clear();
}

Dict::Dict(const Dict & other) : Dict()
{
    for (Dict::const_iterator it=other.begin(); it!=other.end(); it++)
        setPtr(it->first, it->second->clone());
}

Dict & Dict::operator=(const Dict & other)
{
    for (Dict::iterator it=begin(); it!=end(); it++)
        delete it->second;
    _content.clear();
    for (Dict::const_iterator it=other.begin(); it!=other.end(); it++)
        setPtr(it->first, it->second->clone());
    return *this;
}

Dict & Dict::stealMerge(Dict & other)
{
    Dict::iterator it, next;
    for (it=other.begin(); it!=other.end();){
        next = it;
        next++;
        std::string key = it->first;
        setPtr(key, other.steal(key));
        it = next;
    }
    return *this;
}

Type Dict::type(void) const 
{
	return Dict_t;
}

Value * Dict::clone(void) const 
{
	Dict *res = new Dict();
    for (Dict::const_iterator it=begin(); it!=end(); it++)
        res->set(it->first, *(it->second));
    return (Value*) res;
}

void Dict::_writeTo(std::ostream & out) const
{
    std::unordered_map<std::string, Value*>::const_iterator it;
    out << "{";
    for (it=_content.begin(); it!=_content.end(); it++){
        if (it != _content.begin())
            out << ", ";
        out << JSON::String(it->first) << ": " << *(it->second);
    }
    out << "}";
}

bool Dict::hasKey(std::string const & key) const
{
    return _content.find(key) != _content.end();
}

void Dict::setPtr(std::string const & key, Value *ptr)
{
    std::unordered_map<std::string, Value*>::iterator pos = _content.find(key);
    if (pos != _content.end()){
        delete pos->second;
        pos->second = ptr;
    } else {
        _content.insert(
            _content.begin(), 
            std::pair<std::string, Value*>(key, ptr)
        );
    }
}

void Dict::set(std::string const & key, Value const & val)
{
    setPtr(key, val.clone());
}

const Value * Dict::get(std::string const & key) const
{
    std::unordered_map<std::string, Value*>::const_iterator elem;
    elem = _content.find(key);
    if (elem == _content.end()) 
        return NULL;
    return elem->second;
}

Value * Dict::steal(std::string const & key)
{
    std::unordered_map<std::string, Value*>::const_iterator elem;
    elem = _content.find(key);
    if (elem == _content.end()) 
        throw KeyError(key);
    Value *res = elem->second;
    _content.erase(elem);
    return res;

}

void Dict::set(std::string const & key, double val)
{
    if (round(val) == val)
        set(key, Integer(val));
    else
        set(key, Float(val));
}

void Dict::set(std::string const & key, std::string const & val)
{
    set(key, String(val));
}

Dict::iterator Dict::begin(void)
{
    return _content.begin();
}

Dict::iterator Dict::end(void)
{
    return _content.end();
}

Dict::const_iterator Dict::begin(void) const
{
    return _content.begin();
}

Dict::const_iterator Dict::end(void) const
{
    return _content.end();
}

size_t Dict::len(void) const
{
    return _content.size();
}
