#ifndef GUIF_GAME_MANAGER_HPP
#define GUIF_GAME_MANAGER_HPP

#include <network/ClientConnectionManager.hpp>
#include <graphics/GraphicManager.hpp>
#include "FGameManager.hpp"
#include <string>
#include <map>
#include <GUINotifier.hpp>
#include <ctime>

class GUIFGameManager : public FGameManager, public GUI::GraphicManager 
{
public:
	GUIFGameManager(ClientController const & parent, GUI::MainController &controller);
	~GUIFGameManager(){}
private:
	void invitePlayer(std::string playername);
	void onUserList(JSON::List const & list);

	void onLoop();

	NOTIFIABLE
	time_t _lastUpdated;
};

#endif
