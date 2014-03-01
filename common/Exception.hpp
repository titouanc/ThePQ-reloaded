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
class tooManyPlayersException : public std::runtime_error{
public:
	tooManyPlayersException():runtime_error("Too many players"){};
};
class insufficientFundsException : public std::runtime_error{
public:
	insufficientFundsException():runtime_error("Too many players"){};
};

//Adding a player on market
class playerAlreadyOnMarketException : public std::runtime_error{
public:
	playerAlreadyOnMarketException():runtime_error("Player already on market"){};
};
class notEnoughPlayersException : public std::runtime_error{
public:
	notEnoughPlayersException():runtime_error("Not enough players"){};
};

//-------------Team--------------
class PlayerNotFoundException : public std::runtime_error{
public:
	PlayerNotFoundException():runtime_error("Player not found"){};
};

// -----------Match-------------
class UserDeniedException : public std::runtime_error{
public:
	UserDeniedException():runtime_error("Player denied your invitation"){};
};

#endif // __EXCEPTION_HPP
