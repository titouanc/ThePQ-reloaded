#include "CLIStadiumManager.hpp"

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
				loadInstallations();
				_pending++;
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
	}
	else
	{
		cout << "The number you entered is wrong" << endl;
	}
}

void CLIStadiumManager::onInstallationsLoad(JSON::List const & installs)
{
	StadiumManager::onInstallationsLoad(installs);
	_pending--;
	printInstallationsList();
}