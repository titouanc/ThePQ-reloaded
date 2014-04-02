#ifndef DEFINE_CLIMATCHMANAGER_HEADER
#define DEFINE_CLIMATCHMANAGER_HEADER 

#include "MatchController.hpp"
#include <cli/Menu.hpp>
#include <model/Ball.hpp>

class CLIMatchView : public MatchController {
	private:
	public:
		using MatchController::MatchController;

		void selectPlayer();
		void run();

		void onStateChange();

		void onPitchChange();

		void onError(std::string const & info);

		/* Return colored letter for a player (without cleanup escape code) */
		std::string colorPlayerLetter(Player const & player);

		/* Return colored letter for a ball (without cleanup escape code) */
		std::string colorBallLetter(Ball const & ball);

		void displayPitch();
		
		void selectDirectionForPlayer(Player * player);
};

#endif
