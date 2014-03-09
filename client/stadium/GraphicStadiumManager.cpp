#include "GraphicStadiumManager.hpp"
#include <match/GraphicMatchManager.hpp>

using namespace std;
using namespace GUI;

GraphicStadiumManager::GraphicStadiumManager(ClientManager const & parent, GUI::MainController &controller) 
			: StadiumManager(parent), GraphicManager(controller){
	loadInstallations();
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
		&GraphicStadiumManager::displayInstallations, this, "Installations management"
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

void GraphicStadiumManager::displayInstallations()
{
	readMessages();

	_canvas.clear();

	TableView & installations = _canvas.addTableView(2, 10);
	installations.setPosition(100, 100);

	for (size_t i = 0; i < user().installations.size(); ++i){
		TableCell & current = installations.addTableCell(250, 250, sf::Color(0xee, 0xee, 0xee, 0xff));
		current.addLabel(user().installations[i]->getName()).setPosition(10, 10);
		Label &levelLabel = current.addLabel(user().installations[i]->getLevel());	
		levelLabel.setPosition(250-15-levelLabel.getWidth(), 10);
		levelLabel.setColor(BLUE_TEXT_COLOR);
	}

	redrawCanvas();
}

void GraphicStadiumManager::doNothing()
{} 
