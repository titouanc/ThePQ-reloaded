#include "dict.hpp"
#include "string.hpp"
#include "error.hpp"
#include "number.hpp"
#include <cmath>

using namespace JSON;

/// Constructor
Dict::Dict() : _content()
{}

/// Destructor
Dict::~Dict() 
{
    std::unordered_map<std::string, Value*>::iterator elem;
    for (elem=_content.begin(); elem!=_content.end(); elem++)
        delete elem->second;
    _content.clear();
}

/// Copy constructor
Dict::Dict(const Dict & other) : Dict()
{
    for (Dict::const_iterator it=other.begin(); it!=other.end(); it++)
        setPtr(it->first, it->second->clone());
}

Dict::Dict(std::initializer_list<Pair> initial) : Dict()
{
    std::initializer_list<Pair>::iterator it;
    for (it=initial.begin(); it!=initial.end(); it++){
        set(it->key, it->val);
    }
}

/// Operator overloading
Dict & Dict::operator=(const Dict & other)
{
    for (Dict::iterator it=begin(); it!=end(); it++)
        delete it->second;
    _content.clear();
    for (Dict::const_iterator it=other.begin(); it!=other.end(); it++)
        setPtr(it->first, it->second->clone());
    return *this;
}

/**
 * Method performing a merge by "stealing" elements from the 
 * other dictionnary
 * @param Dict : refrence of the dictionnary from whom to merge 
 * @return Dict : reference to the new merged dictionnary
 */
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

/**
 * Method determining the type of the dictionanry
 * @return Type : type of the new dictionnary
*/
Type Dict::type(void) const 
{
	return Dict_t;
}

/**
 * Method creating a Value object
 * @return Value : pointer to a newly created value object
 */

Value * Dict::clone(void) const 
{
	Dict *res = new Dict();
    for (Dict::const_iterator it=begin(); it!=end(); it++)
        res->set(it->first, *(it->second));
    return (Value*) res;
}

/**
 * Method writing to the outoupt stream
 * @param std::ostream  : stream to which to write to
 */
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

/**
 * Method testing if a key belongs to the dictionnary
 * @param std::string : key to check in the dictionnary
 */
bool Dict::hasKey(std::string const & key) const
{
    return _content.find(key) != _content.end();
}

/**
 * Method insert key value pair in dictionnary
 * @param std::string : key to be placed in the dictionnary
 * @param Value : value to be stored with provided key
 */
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

/**
 * Method setting the key - value pair
 * @param std::string : key to be placed in the dictionnary
 * @param Value : value to be stored with the associated key
 */
void Dict::set(std::string const & key, Value const & val)
{
    setPtr(key, val.clone());
}

/**
 * Method retrieving the value of a key
 * @param std::string : key whose value should be returned
 * @return Value : value of the key in the dictionnary
 */
const Value * Dict::get(std::string const & key) const
{
    std::unordered_map<std::string, Value*>::const_iterator elem;
    elem = _content.find(key);
    if (elem == _content.end()) 
        return NULL;
    return elem->second;
}

/**
 * Method stealing a value
 * @param std::string  : key of the value to be stolen (yeah thieves can finally thrive)
 * @return Value * : pointer to the stolen value
 */
Value * Dict::steal(std::string const & key)
{
    std::unordered_map<std::string, Value*>::const_iterator elem;
    elem = _content.find(key);
    if (elem == _content.end()) //key not found
        throw KeyError(key);
    Value *res = elem->second;
    _content.erase(elem);
    return res;
}

/**
 * Method setting a key value pair
 * @param std::string : key whose value will be stored
 * @param double : value of the key to be stored
 */
void Dict::set(std::string const & key, double val)
{
    if (round(val) == val)
        set(key, Integer(val));
    else
        set(key, Float(val));
}

/**
 * Method setting a key value pair
 * @param std::string : key whose value will be stored
 * @param std::string : value of the key to be stored
 */
void Dict::set(std::string const & key, std::string const & val)
{
    set(key, String(val));
}

/**
 * Method handling the start of the dictionnary
 * @return iterator (start pos) 
*/
Dict::iterator Dict::begin(void)
{
    return _content.begin();
}

/**
 * Method handling the end of the dictionnary
 * @return iterator (end pos)
 */
Dict::iterator Dict::end(void)
{
    return _content.end();
}

/**
 * Method handling the start of the dictionnary
 * @return iterator (start pos)
 */
Dict::const_iterator Dict::begin(void) const
{
    return _content.begin();
}

/**
 * Method handling the end of the dictionnary
 * @return iterator (end pos)
 */
Dict::const_iterator Dict::end(void) const
{
    return _content.end();
}

/**
 * Method handling the length of the dictionnary
 * @return size_t : length of the dictionnary
 */
size_t Dict::len(void) const
{
    return _content.size();
}
