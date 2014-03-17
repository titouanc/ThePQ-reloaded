#ifndef DEFINE_JSONLIST_HEADER
#define DEFINE_JSONLIST_HEADER

#include <vector>
#include "value.hpp"

namespace JSON {
	class List : public Value {
        private:
            std::vector<Value*> _content;
            void _writeTo(std::ostream & out) const;
        public:
            List();
            ~List();
            List(List const & other);
            List & operator=(List const & other);

            Type type(void) const;
            Value * clone(void) const;
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
}

#endif