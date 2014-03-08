#include "Client.hpp"
#include <pthread.h>
#include "CLIMatchManager.hpp"

using namespace std;

std::string humanExcName(const char *name)
{
	int status;
	char *str = abi::__cxa_demangle(name, 0, 0, &status);
	std::string res(str);
	free(str);
	return res;
}

Client::Client(NetConfig const &config) : 	_user(), _notifications(), _connection(config.host, config.port),
												_userManager(_connection, _user, _notifications),
												_isRunning(true)
{}

Client::~Client()
{}

void Client::run()
{
	_connection.run();
	_userManager.run();
}

// /* Friendly match menu */
// void Client::showFriendlyMatchMenu()
// {
// 	Menu _menu;
// 	_menu.addToDisplay("   - list all connected players\n");
// 	_menu.addToDisplay("   - choose one to play a friendly game with\n");
// 	_menu.addToDisplay("   - quit to main menu\n");
// 	int option;
// 	do
// 	{
// 		option = _menu.run();
// 		switch(option)
// 		{
// 			case 1:
// 				printConnectedUsersList();
// 				break;
// 			case 2:
// 				showChooseUserMenu();
// 				break;
// 			default:
// 				break;
// 		}
// 	}
// 	while(option != 3);
// }

// void Client::showChooseUserMenu()
// {
// 	string userInput = Menu::askForUserData("Enter a username to send an invitation to another user : ");
// 	_matchManager.chooseUser(userInput);
// 	cout << "Please wait for " << userInput << " to answer to your invitation..." << endl;
// 	try {
// 		_matchManager.waitForUser();
// 		cout << userInput << " accepted your invitation!" << endl;
// 		_matchManager.startMatch();
// 		_matchManager.showTurnMenu();
// 	}
// 	catch (UserNotFoundException &e)
// 	{
// 		cout << "Username not found" << endl;
// 	}
// 	catch (UserDeniedException &e)
// 	{
// 		cout << userInput << " denied your invitation. Sorry!" << endl;		
// 	}
// }

// void Client::printConnectedUsersList(){
// 	vector<std::string> connectedUsers = _matchManager.getConnectedUsersList();
// 	cout << "Here are all the connected users : " << endl;
//  	for (size_t i=0; i < connectedUsers.size(); ++i)
//  		cout << "  - " << connectedUsers[i] << endl;
// }

// void Client::handleFriendlyGameInvitation(JSON::Value const *json){
// 	if (ISSTR(json)){
// 		string user = STR(json).value();
// 		cout << user << " invited you to a game" << endl;
// 		// bool handle = askForNotificationHandling();
// 		bool handle = true;
// 		Menu _menu;
// 		_menu.addToDisplay("   - accept\n");
// 		_menu.addToDisplay("   - deny\n");
// 		int option;
// 		while(handle)
// 		{
// 			option = _menu.run();
// 			if (option == 1){
// 				handle = false;
				
// 				//_matchManager.acceptInvitationFromUser(user);
// 				//showTurnMenu();
// 				// TODO start to play game
// 				CLIMatchManager YAYAYA(_connection, _user, _notifications);
// 				YAYAYA.run();
// 				break;
// 			} else if (option == 2){
// 				handle = false;
// 				_matchManager.denyInvitationFromUser(user);
// 				break;
// 			} else {
// 				cout << "Wrong option entered" << endl;
// 				break;
// 			}
// 		}
// 	}
// }
