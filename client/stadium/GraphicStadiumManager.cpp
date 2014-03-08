#include "GraphicStadiumManager.hpp"
#include <match/GraphicMatchManager.hpp>

using namespace std;
using namespace GUI;

GraphicStadiumManager::GraphicStadiumManager(ClientManager const & parent, GUI::MainController &controller) 
			: StadiumManager(parent), GraphicManager(controller){
	_startMatchButton = _canvas.addButton<GraphicStadiumManager>(&GraphicStadiumManager::startMatch, this, "Start match");
	_startMatchButton->setPosition(1000, 600);
	displayCanvas();
}

void GraphicStadiumManager::startMatch() const {
	// TODO Titou : hook GraphicMatchManager right here.
	GraphicMatchManager match((ClientManager const &)*this, _controller);
	match.run();
}

