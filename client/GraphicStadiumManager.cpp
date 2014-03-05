#include "GraphicStadiumManager.hpp"
#include "GraphicMatchManager.hpp"

using namespace std;
using namespace GUI;

GraphicStadiumManager::GraphicStadiumManager(net::ClientConnectionManager& connection, UserData& user, GUI::MainController &controller) 
			: StadiumManager(connection, user), GraphicManager(controller){
	_startMatchButton = _canvas.addButton<GraphicStadiumManager>(&GraphicStadiumManager::startMatch, this, "Start match");
	_startMatchButton->setPosition(1000, 600);
	displayCanvas();
}

void GraphicStadiumManager::startMatch() const {
	// TODO Titou : hook GraphicMatchManager right here.
	GraphicMatchManager match((ClientManager const &)*this, _controller);
	match.run();
}

