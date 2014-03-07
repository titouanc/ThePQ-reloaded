#ifndef DEFINE_CLI_HEADER
#define DEFINE_CLI_HEADER

#include <iostream>
#include "Menu.hpp"
#include <toolbox.hpp>

class CLI {
protected:
	int _pending;
public:
	void prompt() const;
	bool askForNotificationHandling() const;
	void splashScreen() const;
	void goodBye() const;
};

#endif