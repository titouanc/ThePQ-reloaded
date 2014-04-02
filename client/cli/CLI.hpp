#ifndef DEFINE_CLI_HEADER
#define DEFINE_CLI_HEADER

#include <iostream>
#include "Menu.hpp"
#include <toolbox.hpp>

class CLI {
public:
	void prompt() const;
	void splashScreen() const;
	void goodBye() const;
	void clScreen() const;
	std::ostream & errorMsg(std::string const & message="") const;
	std::ostream & okMsg(std::string const & message="") const;
};

#endif