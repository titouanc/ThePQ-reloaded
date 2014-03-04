#include "GraphicStadiumManager.hpp"

using namespace std;
using namespace GUI;

GraphicStadiumManager::GraphicStadiumManager(net::ClientConnectionManager& connection, UserData& user, GUI::MainController &controller) 
			: StadiumManager(connection, user), GraphicManager(controller){
	_startMatchButton = _canvas.addButton<GraphicStadiumManager>(&GraphicStadiumManager::startMatch, this, "Start match");
	_startMatchButton->setPosition(1000, 600);
	displayCanvas();
	run();
}

void GraphicStadiumManager::startMatch(){
	// TODO Titou : hook GraphicMatchManager right here.
}

