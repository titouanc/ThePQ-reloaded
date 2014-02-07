#include "json.h"
#include <cassert>
#include <cmath>

using namespace JSON;

#define STRIP_JUNK "\r\n\t "
static inline const char *lstrip(const char *str, const char *junk=STRIP_JUNK){
    while (*str && strchr(junk, *str)) 
        str++; 
    return str;
}

static Value *parseNumber(const char *str, char **endptr)
{
    assert('0' <= *str && *str <= '9');
    assert(endptr != NULL);

    double val = strtod(str, endptr);
    if (*endptr && *endptr > str) {
        if (round(val) == val)
            return new JSON::Integer(val);
        else
            return new JSON::Float(val);
    }

    throw ParseError("Unable to parse number");
    return NULL;
}

static std::string parseString(const char *str, char **endptr)
{
    assert(*str == '"');
    assert(endptr != NULL);

    bool escaped = false;

    *endptr = (char*) str+1;
    while (**endptr && (escaped || **endptr != '"')){
        escaped = (! escaped && **endptr == '\\');
        (*endptr)++;
    }
    ssize_t len = *endptr-str-1;
    if (**endptr != '"')
        throw ParseError("Missing trailing '\"'");
    (*endptr)++;
    return std::string(str+1, len);
}

static Dict *parseDict(const char *str, char **endptr)
{
    assert(*str == '{');
    assert(endptr != NULL);

    Dict *res = new Dict();

    while (*str && *str != '}'){
        /* find key */
        str = lstrip(str+1);
        if (*str != '"')
            throw ParseError("Missing key (already have "+res->dumps()+")");
        std::string key = parseString(str, endptr);
        
        /* find value */
        str = lstrip(*endptr);
        if (*str != ':') /* No key:value association */
            throw ParseError("Missing key:val separator");
        Value *val = parse(str+1, endptr);
        if (! val)
            throw ParseError("Missing value for key "+key);

        /* Insert in result */
        res->set(key, *val);
        delete val;

        /* Find next element separator */
        str = lstrip(*endptr);
        if (*str != ',' && *str != '}')
            throw ParseError(
                "Unexpected character (trailing \""+
                std::string(str) + "\")" + *endptr
            );
    }
    return res;
}

static List *parseList(const char *str, char **endptr)
{
    assert(*str == '[');
    assert(endptr != NULL);
    return NULL;
}

Value *JSON::parse(const char *str, char **eptr)
{
    char *endptr;
    if (eptr == NULL)
        eptr = &endptr;

    str = lstrip(str);
    //* return NULL if empty string
    if (! *str) return NULL;

    if ('0' <= *str && *str <= '9')
        return parseNumber(str, eptr);
    if (*str == '"')
        return new String(parseString(str, eptr));
    if (*str == '{')
        return parseDict(str, eptr);
    if (*str == '[')
        return parseList(str, eptr);

    throw ParseError(
        "Unexpected character (trailing \""+
        std::string(str) + "\")"
    );
    return NULL;
}