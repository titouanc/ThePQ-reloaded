#ifndef DEFINE_GRAPHICTEAMMANAGER_HEADER
#define DEFINE_GRAPHICTEAMMANAGER_HEADER

#include "TeamManager.hpp"
#include <graphics/GraphicManager.hpp>
#include <GUINotifier.hpp>

class GraphicTeamManager : public TeamManager, public GUI::GraphicManager {
public:
	GraphicTeamManager(ClientManager const & parent, GUI::MainController &controller);
	~GraphicTeamManager();
private:
	void displayPlayers();

	/*! Displays nicely the positions occupied by the 7 players composing the squad,
	    including buttons to change the player at that position */
	void displaySquad();
	void changePlayerAt(int position);
	void onPlayersLoad();
	bool _wait;

	NOTIFIABLE
};

#endif
