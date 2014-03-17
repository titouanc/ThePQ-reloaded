#ifndef DEFINE_JSONPARSER_HEADER
#define DEFINE_JSONPARSER_HEADER

#include "value.hpp"
#include <string>

namespace JSON {
	/*! Attempt to read a JSON object from given char buffer.
        Return a newly allocated object (must be freed), or throw
        a JSON::Error */
    Value *parse(const char *str, char **eptr=NULL);

    /*! Attempt to read a JSON object from given file.
        Return a newly allocated object (must be freed), or throw
        a JSON::Error */
    Value *load(const char *filename);
    Value *load(std::string filename);

    /*! Attempt to read a JSON object from given file descriptor.
        Return a newly allocated object (must be freed), or throw
        a JSON::Error */
    Value *readFD(int fd);

    /*! Write a JSON object to given file descriptor. Might raise a 
        JSON::Error */
    void writeFD(int fd, JSON::Value const & json);
}

#endif