#ifndef DEFINE_JSON_HEADER
#define DEFINE_JSON_HEADER

/*
 * json.h : Unique header for a JSON library
 *
 * See demo.cpp for an how-to use this lib.
 * 
 * History:
 *    + 2014-02-07: First release (Titou)
 */

#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <vector>
#include <map>

/* Convenience cast macros */
#define INT(obj)   (*((JSON::Integer *)(obj)))
#define FLOAT(obj) (*((JSON::Float *)(obj)))
#define STR(obj)   (*((JSON::String *)(obj)))
#define DICT(obj)  (*((JSON::Dict *)(obj)))
#define LIST(obj)  (*((JSON::List *)(obj)))
#define BOOL(obj)  (*((JSON::Bool *)(obj)))

/* Convenience typecheck macros */
#define ISINT(ptr)   ((ptr) != NULL && (ptr)->type() == JSON::Integer_t)
#define ISFLOAT(ptr) ((ptr) != NULL && (ptr)->type() == JSON::Float_t)
#define ISSTR(ptr)   ((ptr) != NULL && (ptr)->type() == JSON::String_t)
#define ISLIST(ptr)  ((ptr) != NULL && (ptr)->type() == JSON::List_t)
#define ISDICT(ptr)  ((ptr) != NULL && (ptr)->type() == JSON::Dict_t)
#define ISBOOL(ptr)  ((ptr) != NULL && (ptr)->type() == JSON::Boolean_t)

namespace JSON {
    /* Differents kind of JSON objects */
    enum Type {
        Integer_t = 0x01,
        Float_t   = 0x02,
        String_t  = 0x04,
        List_t    = 0x08,
        Dict_t    = 0x10,
        Boolean_t = 0x20
    };

    class Error : public std::runtime_error {
        public: using std::runtime_error::runtime_error;
    };

    /* JSON errors */
    class KeyError : public Error {
        public: using Error::Error;
    };

    class ParseError : public Error {
        public: using Error::Error;
    };

    class IOError : public Error {
        public: using Error::Error;
    };

    /* Abstract common ancestor for all JSON types */
    class Value {
        private:
            virtual void _writeTo(std::ostream & out) const = 0;
        public:
            virtual ~Value(){}
            virtual Value * clone(void) const = 0;
            virtual Type type(void) const = 0;
            virtual std::string dumps(void) const;
            friend std::ostream & operator<<(std::ostream & out, Value const & val)
            {
                val._writeTo(out);
                return out;
            }
            void save(const char *filename) const;
            bool isNumber() const {return (type()&(Integer_t|Float_t)) != 0;}
            bool isAtom() const {
                return (type()&(Integer_t|Float_t|String_t|Boolean_t)) != 0;
            }
            bool isSequence() const {return (type()&(Dict_t|List_t)) != 0;}
    };

    #include "number.hpp"
    
    class String : public Value {
        private:
            std::string _content;
        public:
            String(std::string str);
            ~String();
            Type type(void) const;
            Value * clone(void) const;
            virtual void _writeTo(std::ostream & out) const;
            std::string const & value(void) const;
            operator std::string const &(){return value();}
            operator const char *(){return value().c_str();}
    };

    class List : public Value {
        private:
            std::vector<Value*> _content;
        public:
            List();
            ~List();
            List(List const & other);
            List & operator=(List const & other);

            Type type(void) const;
            Value * clone(void) const;
            virtual void _writeTo(std::ostream & out) const;
            const Value * operator[](size_t index);
            const Value * operator[](size_t index) const;
            Value * steal(size_t index);
            void appendPtr(Value *ptr);
            void append(Value const & obj);
            size_t len(void) const;

            /* Fast setters */
            void append(double val);
            void append(std::string const & val);
    };

    class Dict : public Value {
        private:
            std::map<std::string, Value*> _content;
        public:
            Dict();
            ~Dict();
            Dict(Dict const & other);
            Dict & operator=(Dict const & other);

            Type type(void) const;
            Value * clone(void) const;
            virtual void _writeTo(std::ostream & out) const;
            bool hasKey(std::string const & key) const;
            void setPtr(std::string const & key, Value *ptr);
            void set(std::string const & key, Value const & val);
            const Value * get(std::string const & key) const;
            Value * steal(std::string const & key);
            size_t len(void) const;
            
            /* Fast setters */
            void set(std::string const & key, double val);
            void set(std::string const & key, std::string const & val);

            typedef std::map<std::string, Value*>::iterator iterator;
            typedef std::map<std::string, Value*>::const_iterator const_iterator;
            iterator begin(void);
            iterator end(void);
            const_iterator begin(void) const;
            const_iterator end(void) const;
    };

    Value *parse(const char *str, char **eptr=NULL);

    Value *load(const char *filename);
    Value *load(std::string filename);
}

#endif
