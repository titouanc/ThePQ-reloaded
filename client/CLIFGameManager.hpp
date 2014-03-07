#ifndef DEFINE_CLIFGAMEMANAGER_HEADER
#define DEFINE_CLIFGAMEMANAGER_HEADER 

#include "FGameManager.hpp"
#include "CLI.hpp"

/* Command line friendly game manager */
class CLIFGameManager : public FGameManager, public CLI {
	public:
		using FGameManager::FGameManager;
		CLIFGameManager(ClientManager const & parent);

		void run();

		void onUserList(JSON::List const & list);
		void onOtherAccept(std::string const & name);
		void onOtherDeny(std::string const & name);
		void onUserNotFound(std::string const & name);
};

#endif