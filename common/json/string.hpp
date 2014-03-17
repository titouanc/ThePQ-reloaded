#ifndef DEFINE_JSONSTRING_HEADER
#define DEFINE_JSONSTRING_HEADER

#include "value.hpp"

namespace JSON {
	class String : public Value {
        private:
            std::string _content;
            void _writeTo(std::ostream & out) const;
        public:
            String(std::string str);
            ~String();
            Type type(void) const;
            Value * clone(void) const;
            std::string const & value(void) const;
            operator std::string const &(){return value();}
            operator const char *(){return value().c_str();}
    };
}

#endif