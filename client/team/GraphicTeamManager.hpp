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
	bool _wait;

	void incVelocity(int player);
	void incStrength(int player);
	void incChance(int player);
	void incPrecision(int player);
	void drawUpgradeButton(int x, int y,int player, int ability);
	NOTIFIABLE
};

#endif
