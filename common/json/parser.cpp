#include "json.hpp"
#include <cstring>
#include <cassert>
#include <cmath>
#include <sstream>

using namespace JSON;

#define STRIP_JUNK "\r\n\t "
static inline const char *lstrip(const char *str, const char *junk=STRIP_JUNK){
    while (*str && strchr(junk, *str)) 
        str++; 
    return str;
}

static Value *parseNumber(const char *str, char **endptr)
{
    /*Method returning a parsed number as Value*/
    assert(('0' <= *str && *str <= '9') || *str == '-');
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

static inline unsigned char hex2byte(const char *src){
    /*Method returning the conversion from hex to
     *byte of <<src>>
     **/
    unsigned char res = 0;
    if (src[0] == '\0' || src[1] == '\0')
        throw ParseError("Unexpected end of string in hexadecimal escape seq");
    for (int i=0; i<2; i++){
        if (i) res <<= 4;
        if ('0' <= src[i] && src[i] <= '9')
            res += src[i]-'0';
        else if ('a' <= src[i] && src[i] <= 'f')
            res += 10 + src[i] - 'a';
        else if ('A' <= src[i] && src[i] <= 'F')
            res += 10 + src[i] - 'A';
        else
            throw ParseError(
                std::string("Unexpected character ")
                +src[i]+" in hexadecimal escape seq"
            );
    }
    return res;
}

static std::string parseString(const char *str, char **endptr)
{
    /*Method returning a string from <<str>>*/
    assert(*str == '"');
    assert(endptr != NULL);

    std::stringstream buffer;
    bool escaped = false;

    *endptr = (char*) str+1;
    while (**endptr && (escaped || **endptr != '"')){
        if (! escaped){
            if (**endptr == '\\')
                escaped = true;
            else 
                buffer << **endptr;
        } else if (escaped) {
            switch (**endptr){
                case '\0': throw ParseError("Missing character in escape seq");
                case '\\': buffer << '\\'; break;
                case 'a': buffer << '\a'; break;
                case 'b': buffer << '\b'; break;
                case 'n': buffer << '\n'; break;
                case 'r': buffer << '\r'; break;
                case 't': buffer << '\t'; break;
                case '"': buffer << '"'; break;
                case 'x': 
                    buffer << hex2byte(*endptr+1); 
                    (*endptr) += 2; 
                    break;
                default:
                    throw ParseError("Unexpected escape character");
            }
            escaped = false;
        }
        (*endptr)++;
    }
    if (**endptr != '"')
        throw ParseError("Missing trailing '\"'");
    (*endptr)++;
    return buffer.str();
}

static Dict *parseDict(const char *str, char **endptr)
{
    /*Method returning a Dic pointer from <<str>> object*/
    assert(*str == '{');
    assert(endptr != NULL);

    Dict *res = new Dict();

    while (*str && *str != '}'){
        try {
            /* find key */
            str = lstrip(str+1);
            if (*str == '}')
                break; /* empty dict */
            if (*str != '"')
                throw ParseError(
                    "Missing key (already have "+res->dumps()+
                    ") Trailing: "+str
                );
            std::string key = parseString(str, endptr);
            
            /* find value */
            str = lstrip(*endptr);
            if (*str != ':') /* No key:value association */
                throw ParseError("Missing key:val separator");

            Value *val = parse(str+1, endptr);
            if (! val)
                throw ParseError("Missing value for key "+key);

            /* Insert in result */
            res->setPtr(key, val);

            /* Find next element separator */
            str = lstrip(*endptr);
            if (*str != ',' && *str != '}')
                throw ParseError(
                    "Expecting dict separator (trailing \""+
                    std::string(str) + "\")"
                );
        } catch (ParseError & err){
            delete res;
            throw err;
        }
    }
    *endptr = (char*) str + (*str ? 1 : 0);
    return res;
}

static List *parseList(const char *str, char **endptr)
{
    /*Method returnin List pointer from <<str>> object*/
    assert(*str == '[');
    assert(endptr != NULL);
    //work  list 
    List *res = new List();

    if (str[1] == ']')
        *endptr = (char *) str + 2;
    else {
        while (*str && *str != ']'){
            try {
                Value *item = parse(str+1, endptr);
                if (item)
                    res->appendPtr(item);

                /* Find next element sepatator */
                str = lstrip(*endptr);
                if (*str != ',' && *str != ']')
                    throw ParseError(
                        "Expecting list separator (trailing \""+
                        std::string(str) + "\")"
                    );
            } catch (ParseError & err) {
                delete res;
                throw err;
            }
        }
        *endptr = (char*) str + (*str ? 1 : 0);
    }
    return res;
}

Value *JSON::parse(const char *str, char **eptr)
{
    /*Method returning value pointer from <<str>> object
     * <<str>> can define integer/Dict/List type*/
    char *endptr;
    if (eptr == NULL)
        eptr = &endptr;

    str = lstrip(str);
    //* return NULL if empty string
    if (! *str) return NULL;

    if (strncmp(str, "true", 4) == 0){
        *eptr = (char*) str + 4;
        return new Bool(true);
    }
    if (strncmp(str, "false", 5) == 0){
        *eptr = (char*) str + 5;
        return new Bool(false);
    }
    if (('0' <= *str && *str <= '9') || *str == '-')
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