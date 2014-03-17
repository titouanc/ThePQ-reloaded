#ifndef DEFINE_JSON_HEADER
#define DEFINE_JSON_HEADER

/*
 * json.h : Unique header for a JSON library
 *
 * See demo.cpp for an how-to use this lib.
 * 
 * History:
 *    + 2014-02-07: First release (Titou)
 *    + 2014-02-19: (Titou)
 *         * Various additions merged in master
 *         * _writeTo() privatised in all subclasses
 *    + 2014-03-10: (Titou)
 *         * Convert convenience cast macros to inline functions with typecheck
 */

#include "number.hpp"
#include "string.hpp"
#include "list.hpp"
#include "dict.hpp"
#include "error.hpp"
#include "parser.hpp"

/*! Convenience cast functions with typecheck, especially useful when
   retrieving elements from lists and dictionaries.
   Outside namespace to avoid extra "JSON::" everywhere in the code */

static inline JSON::Bool & BOOL(const JSON::Value *obj){
    if (! ISBOOL(obj) && ! ISINT(obj)) throw JSON::TypeError("Not a boolean");
    return *((JSON::Bool *) obj);
}

static inline JSON::Integer & INT(const JSON::Value *obj){
    if (! ISINT(obj)) throw JSON::TypeError("Not an integer");
    return *((JSON::Integer *) obj);
}

static inline JSON::Float & FLOAT(const JSON::Value *obj){
    if (! ISFLOAT(obj)) throw JSON::TypeError("Not a float");
    return *((JSON::Float *) obj);
}

static inline JSON::String & STR(const JSON::Value *obj){
    if (! ISSTR(obj)) throw JSON::TypeError("Not a string");
    return *((JSON::String *) obj);
}

static inline JSON::List & LIST(const JSON::Value *obj){
    if (! ISLIST(obj)) throw JSON::TypeError("Not a list");
    return *((JSON::List *) obj);
}

static inline JSON::Dict & DICT(const JSON::Value *obj){
    if (! ISDICT(obj)) throw JSON::TypeError("Not a dictionary");
    return *((JSON::Dict *) obj);
}

#endif
