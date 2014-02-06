#ifndef DEFINE_JSON_HEADER
#define DEFINE_JSON_HEADER

#include <iostream>
#include <cstdlib>
#include <cstring>

namespace JSON {
    class Value {
    public:
        virtual std::string dumps(void) const = 0;
        virtual bool isSequence(void) const {return false;}
        virtual bool isLitteral(void) const {return false;}
        virtual bool isNumber(void)   const {return false;}
        virtual bool isString(void)   const {return false;}
    };
    
    class Litteral : public Value {
        virtual bool isLitteral(void) const {return true;}
    };
    
    template <typename T>
    class Number : public Litteral {
    private:
        T _value;
    public:
        Number(T initial) : _value(initial){};
        T const & value(void) const {return _value;}
        double doubleVal(void) const {return (double) _value;}
        long int intVal(void) const {return (long int) _value;}
        bool isInteger(void) const {return intVal() == value();}
        virtual bool isNumber(void) const {return true;}
    };
    
    class Integer : public Number<long int> {
    public:
        using Number<long int>::Number;
        std::string dumps() const {
            char buffer[21];
            sprintf(buffer, "%ld", value());
            return std::string(buffer);
        }
    };
    
    class Float : public Number<double> {
    public:
        using Number<double>::Number;
        std::string dumps() const {
            char buffer[21];
            snprintf(buffer, 21, "%lf", value());
            return std::string(buffer);
        }
    };
    
    class String : public Litteral {
    private:
        std::string _content;
    public:
        String(std::string str) : _content(str){};
        std::string dumps(void) const {
            return "\"" + _content + "\"";
        };
        std::string const & value(void) const {return _content;};
        virtual bool isString(void) const {return true;};
    };
}

#endif
