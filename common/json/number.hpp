#ifndef DEFINE_JSON_NUMBER_HEADER
#define DEFINE_JSON_NUMBER_HEADER

#include "value.hpp"

namespace JSON {
    template <typename T>
    class Number : public Value {
        private:
            T _value;
            virtual void _writeTo(std::ostream & out) const;
        public:
            Number(T initial);
            ~Number();
            T const & value(void) const;
            double doubleVal(void) const;
            long int intVal(void) const;
            Value *clone(void) const;
            operator T const &(){return value();}
    };
    /*==Define std::objects to JSON format==*/
    class Integer : public Number<long int> {
        public:
            using Number<long int>::Number;
            Type type(void) const;
    };

    class Float : public Number<double> {
        public:
            using Number<double>::Number;
            Type type(void) const;
    };

    class Bool : public Number<bool> {
        private:
            void _writeTo(std::ostream & out) const;
        public:
            using Number<bool>::Number;
            Type type(void) const;
    };

    template <typename T>
    Number<T>::Number(T initial) : _value(initial)
    {}

    template <typename T>
    Number<T>::~Number()
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
        if (ISBOOL(this))
            return new Bool(_value);
        if (ISINT(this))
            return new Integer(_value);
        return new Float(_value);
    }

    template <typename T>
    void Number<T>::_writeTo(std::ostream & out) const
    {
        out << _value;
    }
}

#endif
