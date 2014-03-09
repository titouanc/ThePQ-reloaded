#include "GraphicStadiumManager.hpp"
#include <match/GraphicMatchManager.hpp>

using namespace std;
using namespace GUI;

GraphicStadiumManager::GraphicStadiumManager(ClientManager const & parent, GUI::MainController &controller) 
			: StadiumManager(parent), GraphicManager(controller){
	loadInstallations();
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
		&GraphicStadiumManager::displayInstallations, this, "Installations management"
	).setPosition(900, 410);

	_canvas.addButton<GraphicStadiumManager>(
		&GraphicManager::stop, this, "Exit"
	).setPosition(900, 470);
	
	redrawCanvas();
}

void GraphicStadiumManager::displayInstallations()
{
	readMessages();

	_canvas.clear();

	TableView & installations = _canvas.addTableView(2, 10);

	for (size_t i = 0; i < user().installations.size(); ++i){
		TableCell & current = installations.addTableCell(300, 300, sf::Color(0xee, 0xee, 0xee, 0xff));
		current.addLabel(user().installations[i]->getName()).setPosition(10, 10);	
	}

	redrawCanvas();
}

void GraphicStadiumManager::doNothing()
{} 
