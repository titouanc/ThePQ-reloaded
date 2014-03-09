#include "GraphicStadiumManager.hpp"
#include <match/GraphicMatchManager.hpp>

using namespace std;
using namespace GUI;

GraphicStadiumManager::GraphicStadiumManager(ClientManager const & parent, GUI::MainController &controller) 
			: StadiumManager(parent), GraphicManager(controller){
	displayCanvas();
	displayMainMenu();
	readMessages();
}

void GraphicStadiumManager::displayMainMenu()
{
	_canvas.clear();
	
	_canvas.addButton<GraphicStadiumManager>(
		&GraphicStadiumManager::displayPlayers, this, "Players management"
	).setPosition(900, 350);

	_canvas.addButton<GraphicStadiumManager>(
		&GraphicStadiumManager::doNothing, this, "Installations management"
	).setPosition(900, 410);

	_canvas.addButton<GraphicStadiumManager>(
		&GraphicManager::stop, this, "Back"
	).setPosition(900, 470);
	
	redrawCanvas();
}

void GraphicStadiumManager::displayPlayers()
{
	_canvas.clear();

	TableView & playerList = _canvas.addTableView(3);
	playerList.addTableCell(300, 50).addLabel("Name");
	playerList.addTableCell(100, 50).addLabel("Life");
	playerList.addTableCell(100, 50).addLabel("Mana");
	for (Player & player : user().players){
		playerList.addTableCell(300, 30).addLabel(player.getName());
		playerList.addTableCell(100, 30).addLabel(player.getRemainingLife());
		playerList.addTableCell(100, 30).addLabel(player.getRemainingMana());
	}

	_canvas.addButton<GraphicStadiumManager>(
		&GraphicStadiumManager::displayMainMenu, this, "Back"
	).setPosition(900, 650);

	redrawCanvas();
}

void GraphicStadiumManager::doNothing()
{} 
