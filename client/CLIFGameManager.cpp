#include "CLIFGameManager.hpp"
#include "CLI.hpp"
#include "CLIMatchManager.hpp"

#include <iostream>

using namespace std;

CLIFGameManager::CLIFGameManager(ClientManager const & parent) : 
FGameManager(parent)
{}

void CLIFGameManager::onUserList(JSON::List const & list)
{
	cout << "Connected users " << endl;
	for (size_t i=0; i<list.len(); i++){
		if (i > 0)
			cout << ", ";
		cout << STR(list[i]).value();
	}
	cout << endl;
}

void CLIFGameManager::run()
{

}

void CLIFGameManager::onOtherAccept(std::string const & name)
{
	cout << "\033[1m" << name 
		 << " \033[32mhas accepted to play a friendly game !\033[0m" << endl;
	CLIMatchManager match(*this);
	match.run();
}

void CLIFGameManager::onOtherDeny(std::string const & name)
{
	cout << "\033[1m" << name 
		 << " \033[33mdoesn't want to play a friendly game now !\033[0m" << endl;
}

void CLIFGameManager::onUserNotFound(std::string const & name)
{
	cout << "\033[1m" << name 
		 << " \033[31mnot found !\033[0m" << endl;
}