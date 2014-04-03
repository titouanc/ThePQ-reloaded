#ifndef DEFINE_GRAPHICTEAMMANAGER_HEADER
#define DEFINE_GRAPHICTEAMMANAGER_HEADER

#include "TeamController.hpp"
#include <graphics/GraphicManager.hpp>
#include <GUINotifier.hpp>

class GraphicTeamView : public TeamController, public GUI::GraphicManager {
public:
	GraphicTeamView(ClientController const & parent, GUI::MainController &controller);
	~GraphicTeamView();
private:
	void displayPlayers();
	void displaySquadLabels();
	void displayChangeButtons();

	void changePlayerAt(int position);
	void displaySwapWith(int position);
	void displayAvailablePlayers();

	void selectPlayer(int id);
	void doSwap(int id);
	void cancelChange();

	void onPlayersLoad();
	void onSquadUpdated();

	void incVelocity(int player);
	void incStrength(int player);
	void incChance(int player);
	void incPrecision(int player);
	void drawUpgradeButton(int x, int y,int player, int ability);
	NOTIFIABLE
};

#endif
