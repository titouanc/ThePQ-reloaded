#include "json.h"

using namespace JSON;

/* ===== Number<T> ===== */
template <typename T>
Number<T>::Number(T initial) : _value(initial)
{}

template <typename T>
T const & Number<T>::value(void) const 
{
    return _value;
}

template <typename T>
double Number<T>::doubleVal(void) const 
{
    return (double) _value;
}

template <typename T>
long int Number<T>::intVal(void) const 
{
    return (long int) _value;
}

template <typename T>
Value * Number<T>::clone(void) const
{
    if (ISINT(this))
        return new Integer(_value);
    return new Float(_value);
}

/* ===== Integer ===== */
Type Integer::type(void) const 
{
    return Integer_t;
}

std::string Integer::dumps() const 
{
    char buffer[21];
    sprintf(buffer, "%ld", value());
    return std::string(buffer);
}

/* ===== Float ===== */
Type Float::type(void) const 
{
    return Float_t;
}

std::string Float::dumps() const 
{
    char buffer[21];
    snprintf(buffer, 21, "%lf", value());
    return std::string(buffer);
}