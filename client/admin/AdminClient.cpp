#include "AdminClient.hpp"

AdminClient::AdminClient(NetConfig const &config) : _connection(config.host, config.port), _isRunning(true),
_admin()
{
	_connection.run();
	
}

/**
*Method starting the admin interface
*/
void AdminClient::run(){
	
	std::cout<<"Welcome. This is the admin client for The Pro Quidditch Manager 2014."<<std::endl;
	while(_isRunning){
		if(_admin.isLogged()){
			showAdminMenu();
		}
		else{
			_isRunning = showBaseMenu();
		}
	}
	std::cout<<"Quitting the admin client."<<std::endl;
}


/**
 *Method displaying the administration menu
 */
void AdminClient::showAdminMenu(){
	
	Menu _menu;
	_menu.addToDisplay("   - create a championship\n");
	_menu.addToDisplay("   - quit\n");
	int option;
	do{
		option = _menu.run();
		switch(option){
			case 1:
				showCreateChampionshipMenu();
				break;
			case 2:
				logoutAdmin();
			default:
				break;
		}
	}
	while(option!=2);
}

bool AdminClient::showBaseMenu(){
	Menu _menu;
	_menu.addToDisplay("   - identify\n");
	_menu.addToDisplay("   - quit\n");
	int option;
	bool res = true;
	option = _menu.run();
	switch(option)
	{
		case 1:
			showIdentifyMenu();
			break;
		default:
			res = false;
			break;
	}
	return res;
}

/**
 *Method showing the administrator login menu
 */
void AdminClient::showIdentifyMenu(){	
	
	std::string username = Menu::askForUserData("Username : ");
	std::string password = Menu::askForUserData("Password : ");
	
	try {
		std::cout << "Please wait..." << std::endl;
		loginAdmin(username, password);
		std::cout << "You have \033[32msuccessfully\033[0m logged in as administrator." << std::endl;
	}
	catch (const AlreadyLoggedInException& e){
		std::cout << "\033[31mError :\033[0m the admin client is already being used." <<std::endl;
	}
	catch (const  WrongPasswordException& e){
		std::cout << "\033[31mError :\033[0m wrong combination." << std::endl;
	}
	catch(const UserNotFoundException& e){
		std::cout << "\033[31mError :\033[0m wrong combination." << std::endl;
	}
}

/**
 *Method sending the admin credentials to the server.
 * Credential errors are handled by specific exceptions
 */
void AdminClient::loginAdmin(std::string username, std::string password){
	
	JSON::Dict toSend, data;
	data.set(net::MSG::USERNAME, username);
	data.set(net::MSG::PASSWORD, password);
	toSend.set("type", net::MSG::ADMIN_LOGIN);
	toSend.set("data", data);
	_connection.send(toSend);

	JSON::Value *serverMessage = _connection.popMessage();
	JSON::Dict const & received = DICT(serverMessage); 
	if (ISSTR(received.get("data"))) {
		std::string const & res = STR(received.get("data"));
		if (res == net::MSG::PASSWORD_ERROR)
		{
			delete serverMessage;
			throw WrongPasswordException();
		}
		else if (res == net::MSG::USER_NOT_FOUND)
		{
			delete serverMessage;
			throw UserNotFoundException();
		}
		else if (res == net::MSG::ALREADY_LOGGED_IN)
		{
			delete serverMessage;
			throw AlreadyLoggedInException();
		}

	}
	_admin.username =username;
	delete serverMessage;
}

/**
 *Method handling the logout of the admin user
 */
void AdminClient::logoutAdmin(){
	
	_admin.logout();
	JSON::Dict toSend;
	toSend.set("type",net::MSG::DISCONNECT);
	toSend.set("data","");
	std::cout<<toSend<<std::endl;
	_connection.send(toSend);
}

/**
 *Method showing the interface for a championship constructor
 */
void AdminClient::showCreateChampionshipMenu(){	
	
	std::cout << "\n\033[1m\033[33mCreating championship :\033[0m"<<std::endl;
	std::cout<<"Championship name : ";
	std::string champName;
	while(champName.empty()){
		std::getline(cin,champName);
	}
	int nbTurns = 0;
	///verify championship turns are valid
	while(nbTurns <=1 || nbTurns > gameconfig::MAX_CHAMPIONSHIP_TURNS){
		std::cout << "Number of turns (between 2 and " << gameconfig::MAX_CHAMPIONSHIP_TURNS <<") : "; 
		cin >> nbTurns;
	}
	int cashprize = -1;
	///configure championship cashprize
	while(cashprize < 0){
		std::cout << "Cashprize (> 0) : ";
		cin >> cashprize;
	}
	int fame =-1;
	///configure championship fame gain
	while(fame<0){
		std::cout << "Fame (> 0) : ";
		cin >> fame;
	}
	try{
		createChampionship(champName, nbTurns, cashprize, fame);
		std::cout << "Championship \033[32msuccessfully\033[0m created !" << std::endl;
	}
	catch(const ChampionshipNameError & e){
		std::cout << "\033[31mError : \033[0mthat championship name already exists." <<std::endl;
	}
}

/**
 *Method creating a championship
 */
void AdminClient::createChampionship(std::string name, int nbTurns, int cash, int fame){
	
	JSON::Dict toSend;
	toSend.set("type",net::MSG::CHAMPIONSHIP_CREATION);
	JSON::Dict data;
	data.set(net::MSG::CHAMPIONSHIP_NAME,name);
	data.set(net::MSG::TURN_NUMBER,nbTurns);
	data.set(net::MSG::CHAMPIONSHIP_CASHPRIZE,cash);
	data.set(net::MSG::CHAMPIONSHIP_FAME,fame);
	toSend.set("data",data);
	_connection.send(toSend);
	JSON::Value *serverMessage = _connection.popMessage();
	JSON::Dict const & received = DICT(serverMessage);
	if(ISSTR(received.get("data"))){
		std::string response = STR(received.get("data")).value();
		if(response == net::MSG::CHAMPIONSHIP_ALREADY_EXISTS){
			throw ChampionshipNameError();
		}
	}
	delete serverMessage;
}







