#ifndef __TEAM_MANAGER_HPP
#define __TEAM_MANAGER_HPP

#include <vector>
#include <UserData.hpp>
#include <cli/Menu.hpp>
#include <ClientController.hpp>

class TeamController : public ClientController
{
public:
	using ClientController::ClientController;
	TeamController(ClientController const & parent);
	void selectPosition(int position);
	int getSelectedPosition();
	void upgradePlayerAbility(int player_id, int ability);
protected:
	int _selectedPosition; // is the position that we want to change;
	void putPlayerAtPosition(int player_id, int position);

	/* for swapPlayers, we need to select a position and then the player A
	   who is going to go at that position. the player B that was on that position
	   will then go where the player A was. */
	void swapPlayers(int player_id, int position);
};

#endif // __TEAM_MANAGER_HPP
