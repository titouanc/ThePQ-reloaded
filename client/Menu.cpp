#include "Menu.hpp"

using namespace std;

Menu::Menu(string message, std::string choice, void (*callback)(void))
{
	
}

void Menu::setMessage(string message)
{
	
}

void Menu::addOption(std::string choice, void (*callback)(void))
{
	
}

void Menu::run()
{
	string userChoice;
	do
	{
		cout << _message();
		cin >> userChoice;
		map<std::string, void(*)(void)>::const_iterator it;
		for (it = _options.begin(); it != _options.end(); it++)
		{
			
		}
	}
	while (userChoice != "q" && userChoice != "Q");
}
