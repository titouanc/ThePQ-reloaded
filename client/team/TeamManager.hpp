#ifndef __TEAM_MANAGER_HPP
#define __TEAM_MANAGER_HPP

#include <vector>
#include <UserData.hpp>
#include <cli/Menu.hpp>
#include <ClientManager.hpp>

class TeamManager : public ClientManager
{
public:
	using ClientManager::ClientManager;
	TeamManager(ClientManager const & parent);
	void selectPosition(int position);
	int getSelectedPosition();
protected:
	int _selectedPosition; // is the position that we want to change;
	void putPlayerAtPosition(int player_id, int position);
};

#endif // __TEAM_MANAGER_HPP
