#ifndef DEFINE_JSONDICT_HEADER
#define DEFINE_JSONDICT_HEADER

#include <unordered_map>
#include "value.hpp"

namespace JSON {
	struct Pair {
        std::string const & key; 
        Value const & val;
        Pair(std::string const & k, Value const & v) : key(k), val(v) {}
    };
    
    class Dict : public Value {
        private:
            std::unordered_map<std::string, Value*> _content;
            void _writeTo(std::ostream & out) const;
        public:
            Dict();
            ~Dict();
            Dict(Dict const & other);
            Dict(std::initializer_list<Pair> initial);
            Dict & operator=(Dict const & other);

            Type type(void) const;
            Value * clone(void) const;
            bool hasKey(std::string const & key) const;
            void setPtr(std::string const & key, Value *ptr);
            void set(std::string const & key, Value const & val);
            const Value * get(std::string const & key) const;
            Value * steal(std::string const & key);
            size_t len(void) const;
            Dict & stealMerge(Dict & other);
            
            /* Fast setters */
            void set(std::string const & key, double val);
            void set(std::string const & key, std::string const & val);

            typedef std::unordered_map<std::string, Value*>::iterator iterator;
            typedef std::unordered_map<std::string, Value*>::const_iterator const_iterator;
            iterator begin(void);
            iterator end(void);
            const_iterator begin(void) const;
            const_iterator end(void) const;
    };
}

#endif