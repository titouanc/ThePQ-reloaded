#include "json.hpp"
#include <cmath>
#include <sstream>

using namespace JSON;

/// Create empty list
List::List() : _content() 
{}

/// Destructor for List object*/
List::~List() 
{
  
    for (size_t i=0; i<len(); i++)
        delete _content[i];
}

/// Copy constructor
List::List(List const & other) : List()
{
    for (size_t i=0; i<other.len(); i++)
        appendPtr(other[i]->clone());
}

/// Asignment operator overload
List & List::operator=(List const & other)
{
    for (size_t i=0; i<len(); i++)
        delete _content[i];
    _content.clear();
    for (size_t i=0; i<other.len(); i++)
        appendPtr(other[i]->clone());
    return *this;
}

/**
 * Method handling the type of the List object
 * @return Type : type of the elements in the List object
 */
Type List::type(void) const 
{
    return List_t;
}

/**
 * Method creating a deep copy of List
 * @return Value : deep copy pointer of List
 */
Value * List::clone(void) const 
{
    List *res = new List();
    for (size_t i=0; i<len(); i++)
        res->append(*(_content[i]));
    return res;
}

/**
 * Method for writing to ofstream
 * @param std::ostream : stream to which to write to
 */
void List::_writeTo(std::ostream & out) const
{
    
    out << "[";
    for (size_t i=0; i<len(); i++){
        if (i > 0) out << ", ";
        out << *(_content[i]);
    }
    out << "]";
}

/**
 * Method overloading the subscription operator
 * @param size_t : index of the element to return
 * @return Value : value of the element at position index
 */
const Value * List::operator[](size_t index)
{   
    if (index >= len())
        throw KeyError("Index beyond list limits");
    return _content[index];
}

/**
 * Method overloading the subscription operator
 * @param size_t : index of the element to return
 * @return Value : value of the element at position index
 */
const Value * List::operator[](size_t index) const
{
    return _content[index];
}

/**
 * Method returning a pointer to a Value of item at <<index>> of List
 */
Value *List::steal(size_t index)
{
    
    if (index >= len())
        throw KeyError("Index beyond list limits");
    Value *res = _content[index];
    _content.erase(_content.begin()+index);
    return res;
}

/**
 * Method used to append a Value to a List
 * @param Value : value to be appended
 */
void List::appendPtr(Value *ptr)
{
    
    _content.push_back(ptr);
}

/** 
 * Method used to append a Value to a List
 * @param Value : value to be appended
 */
void List::append(Value const & obj)
{
    appendPtr(obj.clone());
}

/**
 * Method returning the size of List
 * @return size_t : length of the list
 */
size_t List::len(void) const 
{
    return _content.size();
}

/** 
 * Method used to append a Value to a List
 * @param Value : value to be appended
 */
void List::append(double val)
{
    if (round(val) == val)
        append(Integer(val));
    else
        append(Float(val));
}

/** 
 * Method used to append a Value to a List
 * @param Value : value to be appended
 */
void List::append(std::string const & val)
{
    append(String(val));
}
