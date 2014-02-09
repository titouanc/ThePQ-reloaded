#ifndef __EXCEPTION_HPP
#define __EXCEPTION_HPP

#include <stdexcept>

// Exceptions
class WrongPasswordException : public std::runtime_error{
public:
	WrongPasswordException():runtime_error("Wrong password"){};
};
class UserNotFoundException : public std::runtime_error{
public:
	UserNotFoundException():runtime_error("User not found"){};
};
class UserAlreadyExistsException : public std::runtime_error{
public:
	UserAlreadyExistsException():runtime_error("User already exists"){};
};

#endif // __EXCEPTION_HPP
