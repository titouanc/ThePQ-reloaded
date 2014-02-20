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
#endif // __EXCEPTION_HPP
