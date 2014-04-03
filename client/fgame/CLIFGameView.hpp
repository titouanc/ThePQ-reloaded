#ifndef DEFINE_CLIFGAMEMANAGER_HEADER
#define DEFINE_CLIFGAMEMANAGER_HEADER 

#include "FGameController.hpp"
#include <cli/CLI.hpp>

/* Command line friendly game manager */
class CLIFGameView : public FGameController, public CLI {
	private:
	public:
		using FGameController::FGameController;
		CLIFGameView(ClientController const & parent);

		void showChooseUserMenu();
		bool showFriendlyMatchMenu();

		void run();

		void onMatchStart();
		void onUserList(JSON::List const & list);
		void onOtherAccept(std::string const & name);
		void onOtherDeny(std::string const & name);
		void onUserNotFound(std::string const & name);
};

#endif