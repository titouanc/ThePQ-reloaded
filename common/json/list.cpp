#include "json.hpp"
#include <cmath>
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

void List::_writeTo(std::ostream & out) const
{
    out << "[";
    for (size_t i=0; i<len(); i++){
        if (i > 0) out << ", ";
        out << *(_content[i]);
    }
    out << "]";
}

const Value * List::operator[](size_t index)
{
    if (index >= len())
        throw KeyError("Index beyond list limits");
    return _content[index];
}

const Value * List::operator[](size_t index) const
{
    return _content[index];
}

Value *List::steal(size_t index)
{
    if (index >= len())
        throw KeyError("Index beyond list limits");
    Value *res = _content[index];
    _content.erase(_content.begin()+index);
    return res;
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


void List::append(double val)
{
    if (round(val) == val)
        append(Integer(val));
    else
        append(Float(val));
}

void List::append(std::string const & val)
{
    append(String(val));
}
