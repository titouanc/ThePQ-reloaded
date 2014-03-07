#ifndef DEFINE_CLIFGAMEMANAGER_HEADER
#define DEFINE_CLIFGAMEMANAGER_HEADER 

#include "FGameManager.hpp"
#include "CLI.hpp"

/* Command line friendly game manager */
class CLIFGameManager : public FGameManager, public CLI {
	private:
	public:
		using FGameManager::FGameManager;
		CLIFGameManager(ClientManager const & parent);

		void showChooseUserMenu();
		bool showFriendlyMatchMenu();

		void run();

		void onUserList(JSON::List const & list);
		void onOtherAccept(std::string const & name);
		void onOtherDeny(std::string const & name);
		void onUserNotFound(std::string const & name);
		void onInvite(std::string username);
};

#endif