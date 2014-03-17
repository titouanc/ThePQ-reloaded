#ifndef DEFINE_JSONERRORS_HEADER
#define DEFINE_JSONERRORS_HEADER

#include <stdexcept>

namespace JSON {
	/*! Base class for all JSON errors */
	class Error : public std::runtime_error {
        public: using std::runtime_error::runtime_error;
    };

    /*! Container missing expected element */
    class KeyError : public Error {
        public: using Error::Error;
    };

    /*! Deserialization encountered a syntax error */
    class ParseError : public Error {
        public: using Error::Error;
    };

    /*! Input/Output related error (filesystem) */
    class IOError : public Error {
        public: using Error::Error;
    };

    /*! Unexpected type */
    class TypeError : public Error {
        public: using Error::Error;
    };
}

#endif
