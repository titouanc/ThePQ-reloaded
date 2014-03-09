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

	backButton().setPosition(900, 470);
	
	redrawCanvas();
}

void GraphicStadiumManager::displayPlayers()
{
	_canvas.clear();

	TableView & playerList = _canvas.addTableView(3, 0);
	/* Header line */
	playerList.addTableCell(300, 50).addLabel("Name");
	playerList.addTableCell(100, 50).addLabel("Life");
	playerList.addTableCell(100, 50).addLabel("Mana");

	/* Content */
	for (Player & player : user().players){
		playerList.addTableCell().addLabel(player.getName());
		playerList.addTableCell().addLabel(player.getRemainingLife());
		playerList.addTableCell().addLabel(player.getRemainingMana());
	}

	backButton().setPosition(900, 650);

	redrawCanvas();
}

void GraphicStadiumManager::doNothing()
{} 
