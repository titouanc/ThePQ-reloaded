#include "GraphicStadiumManager.hpp"
#include <match/GraphicMatchManager.hpp>

using namespace std;
using namespace GUI;

GraphicStadiumManager::GraphicStadiumManager(ClientManager const & parent, GUI::MainController &controller) 
			: StadiumManager(parent), GraphicManager(controller){
	displayCanvas();
	displayMainMenu();
}

void GraphicStadiumManager::displayMainMenu()
{
	_canvas.clear();
	
	_canvas.addButton<GraphicStadiumManager>(
		&GraphicStadiumManager::doNothing, this, "Players management"
	).setPosition(900, 350);

	_canvas.addButton<GraphicStadiumManager>(
		&GraphicStadiumManager::doNothing, this, "Installations management"
	).setPosition(900, 410);

	_canvas.addButton<GraphicStadiumManager>(
		&GraphicManager::stop, this, "Exit"
	).setPosition(900, 470);
	
	redrawCanvas();
}

void GraphicStadiumManager::doNothing()
{} 
