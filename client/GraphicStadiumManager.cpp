#include "GraphicStadiumManager.hpp"

using namespace std;
using namespace GUI;

GraphicStadiumManager::GraphicStadiumManager(net::ClientConnectionManager& connection, UserData& user, GUI::MainController &controller) 
			: StadiumManager(connection, user), GraphicManager(controller){
	_startMatchButton = _canvas.addButton<GraphicStadiumManager>(&GraphicStadiumManager::startMatch, this, "Start match");

	_usernameTextbox->setPosition(900, 300);
	_passwordTextbox->setPosition(900, 360);
	_submitButton->setPosition(900, 420);
	displayCanvas();
	run();
}

void GraphicStadiumManager::startMatch(){
	// TODO Titou : hook GraphicMatchManager right here.
}

