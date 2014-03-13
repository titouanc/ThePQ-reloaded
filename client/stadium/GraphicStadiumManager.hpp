#ifndef GRAPHIC_HOME_MANAGER_HPP
#define GRAPHIC_HOME_MANAGER_HPP

#include <network/ClientConnectionManager.hpp>
#include <graphics/GraphicManager.hpp>
#include "StadiumManager.hpp"

class GraphicStadiumManager : public StadiumManager, public GUI::GraphicManager 
{
public:
	GraphicStadiumManager(ClientManager const & parent, GUI::MainController &controller);
	~GraphicStadiumManager(){}
private:
	void displayInstallations();

	void doUpgrade(int installation);
	void doDowngrade(int installation);
	
	void onInstallationsLoad();
	bool _wait;
};


#endif