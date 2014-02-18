#include "CLI.hpp"

using namespace std;

std::string humanExcName(const char *name)
{
	int status;
	char *str = abi::__cxa_demangle(name, 0, 0, &status);
	std::string res(str);
	free(str);
	return res;
}

CLI::CLI(NetConfig const &config) : 	_prompt(">"), 
												_connection(config.host, config.port)
{
	
}

CLI::~CLI()
{
	
}

void CLI::run()
{
	cout << Message::splashScreen();
	
	loginMenu();
	
	cout << Message::goodBye();
}

void CLI::loginMenu()
{
	/* user menu */
	Menu loginMenu;
	string message;
	message+= "You can : \n";
	message+= "   - (l)ogin\n";
	message+= "   - (r)egister\n";
	message+= "   - (q)uit\n";
	message+= _prompt;
	loginMenu.setMessage(message);
	loginMenu.addOption('l', new ClassCallback<CLI>(this,&CLI::login));
	loginMenu.addOption('r', new ClassCallback<CLI>(this,&CLI::registerUser));

	bool error = false;
	do {
		try{ 
			error = false;
			loginMenu.run(); 
		} catch (std::runtime_error &err){
			error = true;
			cerr << "\033[31mError " << humanExcName(typeid(err).name()) 
				     << "\t" << err.what() << "\033[0m" << endl;
		} catch (...){
			error = true;
			cerr << "\033[31mUnknow error "
				 << "\033[0m" << endl;
		}
	} while (error);
}

void CLI::login(){
	
	string username = askForUserData("Username : ");
	string password = askForUserData("Password : ");
	
	try {
		cout << "Please wait..." << endl;
		_connection.loginUser(username, password);
		cout << "You have successfully logged in! Welcome! :)" << endl;
		mainMenu();
	}
	catch (UserNotFoundException e)
	{
		cout << "User not found" << endl;
	}
	catch (WrongPasswordException e)
	{
		cout << "Wrong password" << endl;
	}
}

void CLI::registerUser(){
	bool registered = false;
	for (int i = 0; i < 3 && ! registered; ++i)
	{
		string username = askForUserData("Pick a username : ");
		try {
			cout << "Please wait..." << endl;
			_connection.doesUserExist(username);
			string password = askForNewPassword();
			cout << "Please wait..." << endl;
			_connection.registerUser(username, password);
			registered = true;
			cout << "You have successfully registered! You can now login." << endl;
		}
		catch (UserAlreadyExistsException e) {
			cout << "User name already exists. Try again with a different username." << endl;		
		}
	}
}
	

/* main menu */
void CLI::mainMenu()
{
	Menu main;
	string message;
	message+= "You can : \n";
	message+= "   - (m)anage your team and stadium\n";
	message+= "   - (p)lay a friendly game\n";
	message+= "   - (q)uit\n";
	message+= _prompt;
	main.setMessage(message);
	main.addOption('m', new ClassCallback<CLI>(this, &CLI::managementMenu));
	main.addOption('p', new ClassCallback<CLI>(this, &CLI::friendlyMatchMenu));
	main.run();
}

/* Management menu */
void CLI::managementMenu()
{
	Menu mgt;
	string message;
	message+= "You can : \n";
	message+= "   - manage your (s)tadium and installations\n";
	message+= "   - manage your (p)layers\n";
	message+= "   - (q)uit to main menu\n";
	message+= _prompt;
	mgt.setMessage(message);
	mgt.addOption('s', new ClassCallback<CLI>(this, &CLI::stadiumMenu));
	mgt.addOption('p', new ClassCallback<CLI>(this, &CLI::playersMenu));
	mgt.run();
}

void CLI::stadiumMenu()
{
	Menu stadium;
	string message;
	message+= "You can : \n";
	message+= "    - (v)iew your installations\n";
	message+= "    - (u)pgrade an installation\n";
	message+= "    - (d)owngrade an installation\n";
	message+= "    - (q)uit to management menu\n";
	stadium.addOption('v', new ClassCallback<CLI>(this, &CLI::printInstallationsList));
	stadium.addOption('u', new ClassCallback<CLI>(this, &CLI::upgradeInstallation));
	stadium.addOption('d', new ClassCallback<CLI>(this, &CLI::downgradeInstallation));
	stadium.setMessage(message);
	// TODO : stadium menu
	stadium.run();
}

void CLI::playersMenu()
{
	Menu players;
	string message;
	message+= "You can : \n";
	message+= "    - (q)uit to management menu\n";
	players.setMessage(message);
	// TODO : players menu
	players.run();
}

/* Friendly match menu */
void CLI::friendlyMatchMenu()
{
	Menu friendly;
	string message;
	message+= "You can : \n";
	message+= "   - (l)ist all connected players\n";
	message+= "   - (c)hoose one to play a friendly game with\n";
	message+= "   - (q)uit to main menu\n";
	message+= _prompt;
	friendly.setMessage(message);
	friendly.addOption('l', new ClassCallback<CLI>(this, &CLI::printConnectedUsersList));
	friendly.addOption('c', new ClassCallback<CLI>(this, &CLI::chooseUser));
	friendly.run();
}

void CLI::chooseUser()
{
	// TODO : choose user for friendly match
}
void CLI::loadInstallations(){
	_installations = _connection.getInstallationsList();
}

void CLI::printInstallationsList(){
	if (_installations.empty())
	{
		loadInstallations();
	}
	// TODO implement printInstallationsList
	cout << "Here are all the installations you own :" << endl;
	for (size_t i = 0; i < _installations.size(); ++i){
		cout << i << " - " << _installations[i].getName() << endl;
		cout << "      Level : 				" << _installations[i].getLevel() << endl;
		cout << "      Current Value : 		" << _installations[i].getCurrentValue() << endl;
		cout << "      Upgrade Cost : 		" << _installations[i].getUpgradeCost() << endl;
		cout << "      Refund Ratio :       " << _installations[i].getRefundRatio() << endl;
		cout << "      Downgrade Refunds : 	" << _installations[i].getDowngradeRefunds() << endl;
	}
}

void CLI::upgradeInstallation()
{
	size_t choice;
	cout << "Enter the number of the installation you want to upgrade" << endl << ">";
	cin >> choice;
	if (choice < _installations.size())
	{
		if (_connection.upgradeInstallation(choice))
		{
			_installations[choice].upgrade();
		}
	}
	else
	{
		cout << "The number you entered is wrong" << endl;
	}
}

void CLI::downgradeInstallation()
{
	size_t choice;
	cout << "Enter the number of the installation you want to downgrade" << endl << ">";
	cin >> choice;
	if (choice < _installations.size())
	{
		if (_connection.downgradeInstallation(choice))
		{
			_installations[choice].downgrade();
		}
	}
	else
	{
		cout << "The number you entered is wrong" << endl;
	}
}

void CLI::printConnectedUsersList(){
	vector<std::string> connectedUsers = _connection.getConnectedUsersList();
	cout << "Here are all the connected users : " << endl;
 	for (size_t i=0; i < connectedUsers.size(); ++i)
 		cout << "  - " << connectedUsers[i] << endl;
}


/* Private methods */

string CLI::askForUserData(string prompt){
	string data;
	cout << prompt;
	cin >> data;
	return data;
}

string CLI::askForNewPassword(){
	string password = "a";
	string passwordConfirmation;
	while (password != passwordConfirmation){
		password = askForUserData("Enter a new password : ");
		passwordConfirmation = askForUserData("Confirm password : ");
		if (password != passwordConfirmation)
			cout << "The two passwords entered were not the same." << endl;
	}
	return password;
}
