#ifndef GRAPHIC_HOME_MANAGER_HPP
#define GRAPHIC_HOME_MANAGER_HPP

#include <network/ClientConnectionManager.hpp>
#include <graphics/GraphicManager.hpp>
#include "StadiumController.hpp"
#include <GUINotifier.hpp>

class GraphicStadiumView : public StadiumController, public GUI::GraphicManager 
{
public:
	GraphicStadiumView(ClientController const & parent, GUI::MainController &controller);
	~GraphicStadiumView(){}
private:
	void displayInstallations();

	void doUpgrade(int installation);
	void doDowngrade(int installation);
	
	void onInstallationsLoad();
	bool _wait;
	NOTIFIABLE
};


#endif