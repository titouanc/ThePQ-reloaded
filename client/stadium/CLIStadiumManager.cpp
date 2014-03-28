#include "CLIStadiumManager.hpp"
#include <match/CLIMatchManager.hpp>

///Constructor
CLIStadiumManager::CLIStadiumManager(ClientManager const & parent) : 
StadiumManager(parent)
{}

/**
 * Method handling the stadium manager
 *all options are delegated to theyr respective handlers
 */
void CLIStadiumManager::run()
{
	clScreen();
	Menu _menu;
	_menu.addToDisplay("    - upgrade an installation\n");
	_menu.addToDisplay("    - downgrade an installation\n");
	_menu.addToDisplay("    - quit to management menu\n");
	int option;
	_pending = 0;
	do
	{
		loadInstallations();
		_pending++;
		do {
			minisleep(0.1);
			readMessages();
		}
		while (_pending > 0);
		printInstallationsList();
		option = _menu.run();
		switch(option)
		{
			case 1:
				showUpgradeInstallation();
				break;
			case 2:
				showDowngradeInstallation();
				break;
			default:
				break;
		}
	}
	while (option != 3);
}

/**
 * Method handling the display of a users installations
 */
void CLIStadiumManager::printInstallationsList()
{
	clScreen();
	cout << "You have " << user().funds << " $$$$" << endl;
	cout << "You have " << user().acPoints << " activity points" << endl;
	cout << "Your fame is " << user().fame << endl;
	cout << "Here are all the installations you own :" << endl;
	for (size_t i = 0; i < user().installations.size(); ++i){
		cout << i << " - \033[32m" << user().installations[i]->getName() << "\033[0m"
			 << " - Level(" << user().installations[i]->getLevel() << "/" << user().installations[i]->getMaxLevel() << ")"
			 << " - Current Value(" << user().installations[i]->getCurrentValue() << ")"
			 << " - Upgrade Cost(" << user().installations[i]->getUpgradeCost() << ")"
			 << " - Income(" << user().installations[i]->getIncome() << ")"
			 << " - Match income per spectator(" << user().installations[i]->getMatchIncome(1) << ")" // TODO
			 << " - Downgrade Refunds(" << user().installations[i]->getDowngradeRefunds() << ")" << endl;
	}
	cout << endl << endl <<endl;
}

/**
 * Method handling the upgrade interface
 */
void CLIStadiumManager::showUpgradeInstallation()
{
	size_t choice;
	cout << "Enter the number of the installation you want to upgrade" << endl;
	prompt();
	cin >> choice;
	if (choice < user().installations.size())
	{
		upgradeInstallation(choice);
		_pending++;
	}
	else
	{
		cout << "The number you entered is wrong" << endl;
	}
}

/**
 * Method handling the downgrade interface for the installations
 */
void CLIStadiumManager::showDowngradeInstallation()
{
	size_t choice;
	cout << "Enter the number of the installation you want to downgrade" << endl;
	prompt();
	cin >> choice;
	if (choice < user().installations.size())
	{
		downgradeInstallation(choice);
		_pending++;
	}
	else
	{
		cout << "The number you entered is wrong" << endl;
	}
}

/**
 * Method handling server queries
 * @param string : type of the query
 * @param JSON::Value : the query
 */
void CLIStadiumManager::treatMessage(std::string const & type, JSON::Value const * data)
{
	StadiumManager::treatMessage(type, data);
	if (type != net::MSG::INSTALLATIONS_LIST)
	{
		_pending++;
	}
	_pending--;
}

/**
 * Method handling the start of a game
 */
void CLIStadiumManager::onMatchStart(){
	CLIMatchManager match(*this); 
	match.run();
}

