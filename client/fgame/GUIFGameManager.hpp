#ifndef GUIF_GAME_MANAGER_HPP
#define GUIF_GAME_MANAGER_HPP

#include <network/ClientConnectionManager.hpp>
#include <graphics/GraphicManager.hpp>
#include "FGameManager.hpp"
#include <string>
#include <map>

class GUIFGameManager : public FGameManager, public GUI::GraphicManager 
{
public:
	GUIFGameManager(ClientManager const & parent, GUI::MainController &controller);
	~GUIFGameManager(){}
private:
	std::map<std::string, GUI::Button<GUIFGameManager, string>*> inviteButtons;
	void invitePlayer(std::string playername);

	void loop();

	void lauchMatch();

	void onOtherAccept(std::string const &);
	void onOtherDeny(std::string const &);
	void onUserList(JSON::List const & list);
	void onInvite(std::string const & otherUser);
};


#endif