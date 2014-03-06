#include "CLIUserManager.hpp"
#include "Menu.hpp"

CLIUserManager::CLIUserManager(ClientManager const & parent) : 
UserManager(parent), _stadiumManager((ClientManager)*this)
{}

void CLIUserManager::run()
{
	/* user menu */
	Menu _menu;
	_menu.addToDisplay("   - login\n");
	_menu.addToDisplay("   - register\n");
	_menu.addToDisplay("   - quit\n");
	int option;
	do {
		option = _menu.run();
		switch(option)
		{
			case 1:
				showLoginMenu();
				break;
			case 2:
				showRegisterMenu();
				break;
			default:
				break;
		}
	}
	while (option != 3);
}

void CLIUserManager::showLoginMenu()
{	
	string username = Menu::askForUserData("Username : ");
	string password = Menu::askForUserData("Password : ");
	
	try {
		cout << "Please wait..." << endl;
		loginUser(username, password);
		cout << "You have successfully logged in! Welcome! :)\n\n\n" << endl;
		showMainMenu();
	}
	catch (NoTeamNameException e)
	{
		cout << "You have successfully logged in! Welcome! :)\n\n\n" << endl;
		showTeamNameMenu();
	}
	catch (UserNotFoundException & e)
	{
		cout << "\nUser not found" << endl;
	}
	catch (WrongPasswordException & e)
	{
		cout << "\nWrong password" << endl;
	}
	catch (AlreadyLoggedInException & e)
	{
		cout << "\nYou're already logged in from another location" << endl;
	}
}

void CLIUserManager::showTeamNameMenu(){
	bool found = false;
	cout << "Hey, it's the first time you log in ! Please, pick up a name for your team." << endl;
	do{
		string teamname = Menu::askForUserData("Teamname : ");
		try{
			chooseTeamName(user().username,teamname);
			cout << "You have successfully picked up a name for your team !\nYou are now the manager of \033[35m"<<teamname<<"\033[0m"<< " !"<<endl;
			found = true;
		}
		catch(TeamNameNotAvailableException e){
			cout << "That teamname is \033[31mnot\033[0m available, pick up another one." << endl;
		}
	}while(!found);
}

void CLIUserManager::showRegisterMenu()
{
	bool registered = false;
	for (int i = 0; i < 3 && ! registered; ++i)
	{
		string username = Menu::askForUserData("Pick a username : ");
		try {
			cout << "Please wait..." << endl;
			doesUserExist(username);
			string password = "a";
			string passwordConfirmation;
			while (password != passwordConfirmation){
				password = Menu::askForUserData("Enter a new password : ");
				passwordConfirmation = Menu::askForUserData("Confirm password : ");
				if (password != passwordConfirmation)
					cout << "The two passwords entered were not the same." << endl;
			}
			cout << "Please wait..." << endl;
			registerUser(username, password);
			registered = true;
			cout << "You have successfully registered! You can now login." << endl;
		}
		catch (UserAlreadyExistsException e) {
			cout << "Username already exists. Try again with a different username." << endl;		
		}
	}
}

void CLIUserManager::showMainMenu()
{
	Menu _menu;
	_menu.addToDisplay("   - manage your team and stadium\n");
	_menu.addToDisplay("   - access market\n");
	_menu.addToDisplay("   - play a friendly game\n");
	_menu.addToDisplay("   - see notifications\n");
	_menu.addToDisplay("   - quit\n");
	int option;
	do
	{
		option = _menu.run();
		switch(option)
		{
			case 1:
				showManagementMenu();
				break;
			case 2:
				// showMarketMenu();
				break;
			case 3:
				// _matchManager.showFriendlyMatchMenu();
				break;
			case 4:
				// showNotificationsMenu();
				break;
			case 5:
				logoutUser();
			default:
				break;
		}
	}
	while (option != 5);
}

/* Management menu */
void CLIUserManager::showManagementMenu()
{
	Menu _menu;
	_menu.addToDisplay("   - manage your stadium and installations\n");
	_menu.addToDisplay("   - manage your players\n");
	_menu.addToDisplay("   - quit to main menu\n");
	int option;
	do
	{
		option = _menu.run();
		switch(option)
		{
			case 1:
				_stadiumManager.run();
				break;
			case 2:
				// showTeamMenu();
				break;
			default:
				break;
		}
	}
	while (option != 3);
}
