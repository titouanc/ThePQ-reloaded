#ifndef GRAPHIC_HOME_MANAGER_HPP
#define GRAPHIC_HOME_MANAGER_HPP

#include <network/ClientConnectionManager.hpp>
#include "graphics/GraphicManager.hpp"
#include "StadiumManager.hpp"

class GraphicStadiumManager : public StadiumManager, public GUI::GraphicManager 
{
public:
	GraphicStadiumManager(net::ClientConnectionManager& connection, UserData& user, GUI::MainController &controller);
	~GraphicStadiumManager(){}
private:
	void startMatch();
	GUI::Button<GraphicStadiumManager> *_startMatchButton;
};


#endif