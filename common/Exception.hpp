#ifndef __EXCEPTION_HPP
#define __EXCEPTION_HPP

#include <stdexcept>

//==========Exceptions===========

//-----Login------
class WrongPasswordException : public std::runtime_error{
public:
	WrongPasswordException():runtime_error("Wrong password"){};
};

class UserNotFoundException : public std::runtime_error{
public:
	UserNotFoundException():runtime_error("User not found"){};
};

class AlreadyLoggedInException : public std::runtime_error {
public:
	AlreadyLoggedInException():runtime_error("Already logged in"){};
};


//------Register------
class UserAlreadyExistsException : public std::runtime_error{
public:
	UserAlreadyExistsException():runtime_error("User already exists"){};
};

//-----PlayerMarket------
//Placing a bid
class bidValueNotUpdatedException : public std::runtime_error{
public:
	bidValueNotUpdatedException():runtime_error("Wrong bid value"){};
};

class turnException : public std::runtime_error{
public:
	turnException():runtime_error("Did not bid last turn"){};
};

class bidEndedException : public std::runtime_error{
public:
	bidEndedException():runtime_error("Bid ended"){};
};


class bidOnYourPlayerException : public std::runtime_error{
public:
	bidOnYourPlayerException():runtime_error("Bid on your player"){};
};

class lastBidderException : public std::runtime_error{
public:
	lastBidderException():runtime_error("Last bidder"){};
};

//Adding a player on market
class playerAlreadyOnMarketException : public std::runtime_error{
public:
	playerAlreadyOnMarketException():runtime_error("Player already on market"){};
};

//-------Memory Access-----------
class wrongLoadFunctionException : public std::runtime_error{
public:
	wrongLoadFunctionException():runtime_error("Bad function call for required datatype"){};
};

class wrongRemoveFunctionException : public std::runtime_error{
public:
	wrongRemoveFunctionException():runtime_error("Bad function call for required datatype"){};
};

class IncompleteDictException : public std::runtime_error{
public:
	IncompleteDictException():runtime_error("Incomplete or wrong dict for required data type"){};
};

#endif // __EXCEPTION_HPP
