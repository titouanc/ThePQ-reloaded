#ifndef DEFINE_CLI_HEADER
#define DEFINE_CLI_HEADER

#include <iostream>
#include "Menu.hpp"

class CLI {
public:
	void prompt() const;
	bool askForNotificationHandling() const;
	void splashScreen() const;
	void goodBye() const;
};

#endif