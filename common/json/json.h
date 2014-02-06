#ifndef DEFINE_JSON_HEADER
#define DEFINE_JSON_HEADER

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>

#define CLONE_METHOD(type) Value * clone(void) const {return new type(value());}

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
            Type type(void) const {return List_t;}
            Value * clone(void) const {return NULL;}
            std::string dumps(void) const {return "Hello";}
            const Value * operator[](size_t index){return _content[index];}
            void append(Value const & obj){_content.push_back(obj.clone());}
            size_t len(void) const {return _content.size();}
    };
}

#endif
