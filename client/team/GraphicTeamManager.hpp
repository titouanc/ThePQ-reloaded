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
	void onPlayersLoad();
	bool _wait;

	NOTIFIABLE
};

#endif
