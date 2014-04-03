#ifndef GUIF_GAME_MANAGER_HPP
#define GUIF_GAME_MANAGER_HPP

#include <network/ClientConnectionManager.hpp>
#include <graphics/GraphicManager.hpp>
#include "FGameController.hpp"
#include <string>
#include <map>
#include <GUINotifier.hpp>
#include <ctime>

class GraphicFGameView : public FGameController, public GUI::GraphicManager 
{
public:
	GraphicFGameView(ClientController const & parent, GUI::MainController &controller);
	~GraphicFGameView(){}
private:
	void invitePlayer(std::string playername);
	void onUserList(JSON::List const & list);

	void onLoop();

	NOTIFIABLE
	time_t _lastUpdated;
};

#endif
