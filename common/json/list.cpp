#include "json.hpp"
#include <cmath>
#include <sstream>

using namespace JSON;

List::List() : _content() //create empty list
{}

List::~List() 
{
    /*Destructor for List object*/
    for (size_t i=0; i<len(); i++)
        delete _content[i];
}

List::List(List const & other) : List()
{
    /*Copy constructor*/
    for (size_t i=0; i<other.len(); i++)
        appendPtr(other[i]->clone());
}

List & List::operator=(List const & other)
{
    /*Assignment operator*/
    for (size_t i=0; i<len(); i++)
        delete _content[i];
    _content.clear();
    for (size_t i=0; i<other.len(); i++)
        appendPtr(other[i]->clone());
    return *this;
}

Type List::type(void) const 
{
    /*Method returning the type*/
    return List_t;
}

Value * List::clone(void) const 
{
    /*Method creating a deep copy of List*/
    List *res = new List();
    for (size_t i=0; i<len(); i++)
        res->append(*(_content[i]));
    return res;
}

void List::_writeTo(std::ostream & out) const
{
    /*Method for writing to ofstream*/
    out << "[";
    for (size_t i=0; i<len(); i++){
        if (i > 0) out << ", ";
        out << *(_content[i]);
    }
    out << "]";
}

const Value * List::operator[](size_t index)
{
    /*Method returning the value at <<index>> of List*/
    if (index >= len())
        throw KeyError("Index beyond list limits");
    return _content[index];
}

const Value * List::operator[](size_t index) const
{
    /*Method returning a constant reference to the value at <<index>> of list*/
    return _content[index];
}

Value *List::steal(size_t index)
{
    /*Method returning a pointer to a Value of item at <<index>> of List*/
    if (index >= len())
        throw KeyError("Index beyond list limits");
    Value *res = _content[index];
    _content.erase(_content.begin()+index);
    return res;
}

void List::appendPtr(Value *ptr)
{
    /*Method used to append a Value to a List*/
    _content.push_back(ptr);
}

void List::append(Value const & obj)
{
    /*Method used to append a Value to a List*/
    appendPtr(obj.clone());
}

size_t List::len(void) const 
{
    /*Method returning the size of List*/
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
