#include "CLIStadiumManager.hpp"
#include <match/CLIMatchManager.hpp>
CLIStadiumManager::CLIStadiumManager(ClientManager const & parent) : 
StadiumManager(parent)
{}

void CLIStadiumManager::run()
{
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

void CLIStadiumManager::printInstallationsList()
{
	clScreen();
	cout << "You have " << user().funds << " $$$$" << endl;
	cout << "You have " << user().acPoints << " activity points" << endl;
	cout << "Your fame is " << user().fame << endl;
	cout << "Here are all the installations you own :" << endl;
	for (size_t i = 0; i < user().installations.size(); ++i){
		cout << i << " - \033[32m" << user().installations[i]->getName() << "\033[0m"
			 << " - Level : " << user().installations[i]->getLevel()
			 << " - Current Value : " << user().installations[i]->getCurrentValue()
			 << " - Upgrade Cost : " << user().installations[i]->getUpgradeCost()
			 << " - Refund Ratio : " << user().installations[i]->getRefundRatio()
			 << " - Downgrade Refunds : " << user().installations[i]->getDowngradeRefunds() << endl;
	}
	cout << endl << endl <<endl;
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

void CLIStadiumManager::clScreen(){
	/*Method*/
	cout << "\033[2J\033[1;1H";
}