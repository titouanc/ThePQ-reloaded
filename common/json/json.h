#ifndef DEFINE_JSON_HEADER
#define DEFINE_JSON_HEADER

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <sstream>
#include <map>
#include <stdexcept>

#define CLONE_METHOD(type) Value * clone(void) const {return new type(value());}

#define INT(obj)   (*((JSON::Integer *)(obj)))
#define FLOAT(obj) (*((JSON::Float *)(obj)))
#define STR(obj)   (*((JSON::String *)(obj)))
#define DICT(obj)  (*((JSON::Dict *)(obj)))
#define LIST(obj)  (*((JSON::List *)(obj)))

namespace JSON {
    enum Type {
        Integer_t = 0x01,
        Float_t   = 0x02,
        String_t  = 0x04,
        List_t    = 0x08,
        Dict_t    = 0x10,
    };

    class Value {
        public:
            virtual ~Value(){}
            virtual Value * clone(void) const = 0;
            virtual std::string dumps(void) const = 0;
            virtual Type type(void) const = 0;
            bool isNumber() const {return (type()&(Integer_t|Float_t)) != 0;}
            bool isAtom() const {return (type()&(Integer_t|Float_t|String_t)) != 0;}
            bool isSequence() const {return (type()&(Dict_t|List_t)) != 0;}
    };

    template <typename T>
    class Number : public Value {
        private:
            T _value;
        public:
            Number(T initial) : _value(initial){};
            T const & value(void) const {return _value;}
            double doubleVal(void) const {return (double) _value;}
            long int intVal(void) const {return (long int) _value;}
    };
    
    class Integer : public Number<long int> {
        public:
            using Number<long int>::Number;
            Type type(void) const {return Integer_t;}
            CLONE_METHOD(Integer)
            std::string dumps() const {
                char buffer[21];
                sprintf(buffer, "%ld", value());
                return std::string(buffer);
            }
    };
    
    class Float : public Number<double> {
        public:
            using Number<double>::Number;
            Type type(void) const {return Float_t;}
            CLONE_METHOD(Float)
            std::string dumps() const {
                char buffer[21];
                snprintf(buffer, 21, "%lf", value());
                return std::string(buffer);
            }
    };
    
    class String : public Value {
        private:
            std::string _content;
        public:
            String(std::string str) : _content(str){};
            Type type(void) const {return String_t;}
            CLONE_METHOD(String)
            std::string dumps(void) const {return "\"" + _content + "\"";}
            std::string const & value(void) const {return _content;}
    };

    class List : public Value {
        private:
            std::vector<Value*> _content;
        public:
            List() : _content() {};
            ~List() {
                for (size_t i=0; i<len(); i++)
                    delete _content[i];
            }
            Type type(void) const {return List_t;}
            Value * clone(void) const {
                List *res = new List();
                for (size_t i=0; i<len(); i++)
                    res->append(*(_content[i]));
                return res;
            }
            std::string dumps(void) const {
                std::stringstream res;
                res << "[";
                for (size_t i=0; i<len(); i++){
                    if (i > 0) res << ", ";
                    res << _content[i]->dumps();
                }
                res << "]";
                return res.str();
            }
            const Value * operator[](size_t index){return _content[index];}
            void appendPtr(Value *ptr){_content.push_back(ptr);}
            void append(Value const & obj){appendPtr(obj.clone());}
            size_t len(void) const {return _content.size();}
    };

    class KeyError : public std::runtime_error {
        public: using std::runtime_error::runtime_error;
    };

    class Dict : public Value {
        private:
            std::map<std::string, Value*> _content;
        public:
            Dict() : _content() {};
            ~Dict() {
                std::map<std::string, Value*>::iterator elem;
                for (elem=_content.begin(); elem!=_content.end(); elem++)
                    delete elem->second;
                _content.clear();
            }
            Type type(void) const {return Dict_t;}
            Value * clone(void) const {return NULL;}
            std::string dumps(void) const {
                std::stringstream res;
                res << "{";
                std::map<std::string, Value*>::const_iterator elem;
                for (elem=_content.begin(); elem!=_content.end(); elem++){
                    if (elem != _content.begin()) res << ", ";
                    res << "\"" << elem->first << "\": " 
                        << elem->second->dumps();
                }
                res << "}";
                return res.str();
            }
            bool hasKey(std::string const & key){
                return _content.find(key) != _content.end();
            }
            void setPtr(std::string const & key, Value *ptr){
                _content.insert(
                    _content.begin(), 
                    std::pair<std::string, Value*>(key, ptr)
                );
            }
            void set(std::string const & key, Value const & val){
                setPtr(key, val.clone());
            }
            const Value * get(std::string const & key){
                std::map<std::string, Value*>::const_iterator elem;
                elem = _content.find(key);
                if (elem == _content.end()) 
                    throw KeyError(key);
                return elem->second;
            }
    };

    class ParseError : public std::runtime_error {
        public: using std::runtime_error::runtime_error;
    };

    Value *parse(const char *str, char **eptr=NULL);
}

#endif
