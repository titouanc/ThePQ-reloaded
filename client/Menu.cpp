#include "Menu.hpp"

Menu::Menu()
{
	_message = "";
}


Menu::~Menu()
{
	typename
	std::map<char, Callback*>::iterator it = _options.begin();
	while (it != _options.end())
	{
		delete it->second;
		it++;
	}
}


void Menu::setMessage(std::string message)
{
	_message = message;
}


void Menu::addOption(char choice, Callback* callback)
{
	_options.insert(std::pair<char, Callback*>(choice, callback));
}


void Menu::run()
{
	char userChoice;
	do
	{
		std::cout << _message;
		std::cin >> userChoice;
		if (userChoice >= 'A' && userChoice <= 'Z')
		{
			userChoice -= 'A' - 'a';
		}
		std::map<char, Callback*>::const_iterator it;
		it = _options.find(userChoice);
		if (it != _options.end())
		{
			(*(it->second))();
		}
	}
	while (userChoice != 'q' && userChoice != 'Q');
}
