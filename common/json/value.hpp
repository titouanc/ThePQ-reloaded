#ifndef DEFINE_JSONVALUE_HEADER
#define DEFINE_JSONVALUE_HEADER

#include <iostream>

/* Convenience typecheck macros */
#define ISINT(ptr)   ((ptr) != NULL && (ptr)->type() == JSON::Integer_t)
#define ISFLOAT(ptr) ((ptr) != NULL && (ptr)->type() == JSON::Float_t)
#define ISSTR(ptr)   ((ptr) != NULL && (ptr)->type() == JSON::String_t)
#define ISLIST(ptr)  ((ptr) != NULL && (ptr)->type() == JSON::List_t)
#define ISDICT(ptr)  ((ptr) != NULL && (ptr)->type() == JSON::Dict_t)
#define ISBOOL(ptr)  ((ptr) != NULL && (ptr)->type() == JSON::Boolean_t)

namespace JSON {
	/*! Differents kind of JSON objects */
    enum Type {
        Integer_t = 0x01,
        Float_t   = 0x02,
        String_t  = 0x04,
        List_t    = 0x08,
        Dict_t    = 0x10,
        Boolean_t = 0x20
    };

	/*! Abstract common ancestor for all JSON types */
    class Value {
        private:
        	/*! Write itself to given stream. 
        	    Used by operator<< and dumps() */
            virtual void _writeTo(std::ostream & out) const = 0;

        public:
            virtual ~Value(){}
            /*! Return a newly allocated deep copy of self */
            virtual Value * clone(void) const = 0;

            /*! Return object type */
            virtual Type type(void) const = 0;

            /*! Return a serialized string of self */
            std::string dumps(void) const;

            /*! Conventionnal operator<< */
            friend std::ostream & operator<<(std::ostream & out, Value const & val)
            {
                val._writeTo(out);
                return out;
            }

            /*! Serialize self to disk. If the file doesn't exists, create it 
                first. If the file is not empty, truncate it first. */
            void save(const char *filename) const;
            void save(std::string const & filename) const;

            /*! Return true if it is a numeric object 
                (ie. useable in arithmetic operations) */
            bool isNumber() const {return (type()&(Integer_t|Float_t)) != 0;}

            /*! Return true if it is not a container */
            bool isAtom() const {
                return (type()&(Integer_t|Float_t|String_t|Boolean_t)) != 0;
            }

            /*! Return true if it is a container */
            bool isSequence() const {return (type()&(Dict_t|List_t)) != 0;}
    };
}

#endif
