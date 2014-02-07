#include "json.h"
#include <sstream>

using namespace JSON;

List::List() : _content() 
{}

List::~List() 
{
    for (size_t i=0; i<len(); i++)
        delete _content[i];
}

Type List::type(void) const 
{
    return List_t;
}

Value * List::clone(void) const 
{
    List *res = new List();
    for (size_t i=0; i<len(); i++)
        res->append(*(_content[i]));
    return res;
}

std::string List::dumps(void) const 
{
    std::stringstream res;
    res << "[";
    for (size_t i=0; i<len(); i++){
        if (i > 0) res << ", ";
        res << _content[i]->dumps();
    }
    res << "]";
    return res.str();
}

const Value * List::operator[](size_t index)
{
    return _content[index];
}

void List::appendPtr(Value *ptr)
{
    _content.push_back(ptr);
}

void List::append(Value const & obj)
{
    appendPtr(obj.clone());
}

size_t List::len(void) const 
{
    return _content.size();
}
