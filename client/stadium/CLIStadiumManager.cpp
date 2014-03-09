#include "CLIStadiumManager.hpp"
#include <match/CLIMatchManager.hpp>
CLIStadiumManager::CLIStadiumManager(ClientManager const & parent) : 
StadiumManager(parent)
{}

void CLIStadiumManager::run()
{
	Menu _menu;
	_menu.addToDisplay("    - view your installations\n");
	_menu.addToDisplay("    - upgrade an installation\n");
	_menu.addToDisplay("    - downgrade an installation\n");
	_menu.addToDisplay("    - quit to management menu\n");
	int option;
	_pending = 0;
	loadInstallations();
	_pending++;
	do
	{
		do {
			minisleep(0.1);
			readMessages();
		}
		while (_pending > 0);
		option = _menu.run();
		switch(option)
		{
			case 1:
				printInstallationsList();
				break;
			case 2:
				showUpgradeInstallation();
				break;
			case 3:
				showDowngradeInstallation();
				break;
			default:
				break;
		}
	}
	while (option != 4);
}

void CLIStadiumManager::printInstallationsList()
{
	cout << "You have " << user().funds << "$$$$" << endl;
	cout << "Here are all the installations you own :" << endl;
	for (size_t i = 0; i < user().installations.size(); ++i){
		cout << i << " - " << user().installations[i]->getName() << endl;
		cout << "      Level : 				" << user().installations[i]->getLevel() << endl;
		cout << "      Current Value : 		" << user().installations[i]->getCurrentValue() << endl;
		cout << "      Upgrade Cost : 		" << user().installations[i]->getUpgradeCost() << endl;
		cout << "      Refund Ratio :       " << user().installations[i]->getRefundRatio() << endl;
		cout << "      Downgrade Refunds : 	" << user().installations[i]->getDowngradeRefunds() << endl;
	}
}

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

void CLIStadiumManager::treatMessage(std::string const & type, JSON::Value const * data)
{
	StadiumManager::treatMessage(type, data);
	if (type != net::MSG::INSTALLATIONS_LIST)
	{
		_pending++;
	}
	_pending--;
}

void CLIStadiumManager::onMatchStart(){
	CLIMatchManager match(*this); 
	match.run();
}