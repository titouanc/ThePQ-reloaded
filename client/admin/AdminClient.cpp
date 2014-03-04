#include "AdminClient.hpp"

AdminClient::AdminClient(NetConfig const &config) : _connection(config.host, config.port), _isRunning(true),
_admin()
{
	JSON::Dict identify;
	identify.set("type",net::MSG::ADMIN_CLIENT);
	identify.set("data","");
	_connection.send(identify);
}

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

void AdminClient::showAdminMenu(){
	Menu _menu;
	_menu.addToDisplay("   - create a championship\n");
	_menu.addToDisplay("   - quit\n");
	int option;
	option = _menu.run();
	do{
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

void AdminClient::showIdentifyMenu(){	
	std::string username = Menu::askForUserData("Username : ");
	std::string password = Menu::askForUserData("Password : ");
	
	try {
		std::cout << "Please wait..." << std::endl;
		loginAdmin(username, password);
		std::cout << "You have successfully logged in as administrator." << std::endl;
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

void AdminClient::loginAdmin(std::string username, std::string password)
{
	JSON::Dict toSend, data;
	data.set(net::MSG::USERNAME, username);
	data.set(net::MSG::PASSWORD, password);
	toSend.set("type", net::MSG::ADMIN_LOGIN);
	toSend.set("data", data);
	_connection.send(toSend);

	JSON::Value *serverMessage = _connection.waitForMsg(net::MSG::STATUS);
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
	_admin.login(username);
	delete serverMessage;
}

void AdminClient::logoutAdmin(){
	_admin.logout();
	JSON::Dict toSend;
	toSend.set("type",net::MSG::DISCONNECT);
	toSend.set("data","");
	std::cout<<toSend<<std::endl;
	_connection.send(toSend);
}

void AdminClient::showCreateChampionshipMenu(){	
	JSON::Dict toSend;
	toSend.set("type",net::MSG::CHAMPIONSHIP_CREATION);
	toSend.set("data",JSON::Dict());
	_connection.send(toSend);
}