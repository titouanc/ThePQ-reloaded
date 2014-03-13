#include "GraphicTeamManager.hpp"

using namespace std;
using namespace GUI;

GraphicTeamManager::GraphicTeamManager(
	ClientManager const & parent, 
	GUI::MainController &controller
) : 
	TeamManager(parent), 
	GraphicManager(controller)
{
	_canvas.setBackgroundImage(texturePath("HexBack.png"));
	displayCanvas();
	if (user().players.size() > 0){
		displayPlayers();
	} else {
		loadPlayers();
		_wait = true;
		while (_wait){
			readMessages();
			readEvent();
		}
	}
}

GraphicTeamManager::~GraphicTeamManager(){}

void GraphicTeamManager::displayPlayers()
{
	_canvas.clear();

	usernameButton(user().username);
	userBudgetButton(user().funds);

	TableView & playerList = _canvas.addTableView();
	playerList.setPosition(100, 100);
	/* Header line */
	TableCell & header = playerList.addTableCell(515, 47);
	header.addLabel("Name", sf::Color::White).setPosition(15, 10);
	header.addLabel("Life", sf::Color::White).setPosition(355, 10);
	header.addLabel("Mana", sf::Color::White).setPosition(435, 10);
	header.setBackgroundColor(BUTTON_BACKGROUND_COLOR);

	/* Content */
	for (Player & player : user().players){
		TableCell & playerCell = playerList.addTableCell(515, 47, sf::Color(0xee, 0xee, 0xee, 0xff));
		playerCell.setBackgroundColor(sf::Color(0x00, 0x00, 0x00, 0x77));
		playerCell.addLabel(player.getName(), sf::Color::White).setPosition(15, 10);;
		playerCell.addLabel(player.getRemainingLife(), GREEN_TEXT_COLOR
			).setPosition(355, 10);
		playerCell.addLabel(player.getRemainingMana(), BLUE_TEXT_COLOR
			).setPosition(435, 10);
	}

	displaySquadLabels();
	displayChangeButtons();

	backButton();

	redrawCanvas();
}

void GraphicTeamManager::displaySquadLabels(){
	_canvas.addPanel(515, 500, sf::Color(0x00, 0x00, 0x00, 0x77)
		).setPosition(650, 100);

	_canvas.addLabel("Left Chaser", sf::Color::White
		).centerOn(755, 160);
	_canvas.addLabel("Center Chaser", sf::Color::White
		).centerOn(900, 160);
	_canvas.addLabel("Right Chaser", sf::Color::White
		).centerOn(1045, 160);

	_canvas.addLabel("Left Beater", sf::Color::White
		).centerOn(816, 265);
	_canvas.addLabel("Right Beater", sf::Color::White
		).centerOn(983, 265);

	_canvas.addLabel("Seeker", sf::Color::White
		).centerOn(900, 370);
	_canvas.addLabel("Keeper", sf::Color::White
		).centerOn(900, 485);
}

void GraphicTeamManager::displayChangeButtons(){
	_canvas.addButton<GraphicTeamManager, int>(
		&GraphicTeamManager::changePlayerAt, 0, this, "todo"
	).centerOn(755, 210);
	_canvas.addButton<GraphicTeamManager, int>(
		&GraphicTeamManager::changePlayerAt, 1, this, "todo"
	).centerOn(900, 210);
	_canvas.addButton<GraphicTeamManager, int>(
		&GraphicTeamManager::changePlayerAt, 2, this, "todo"
	).centerOn(1045, 210);

	_canvas.addButton<GraphicTeamManager, int>(
		&GraphicTeamManager::changePlayerAt, 3, this, "todo"
	).centerOn(816, 315);
	_canvas.addButton<GraphicTeamManager, int>(
		&GraphicTeamManager::changePlayerAt, 4, this, "todo"
	).centerOn(983, 315);

	_canvas.addButton<GraphicTeamManager, int>(
		&GraphicTeamManager::changePlayerAt, 5, this, "todo"
	).centerOn(900, 420);
	_canvas.addButton<GraphicTeamManager, int>(
		&GraphicTeamManager::changePlayerAt, 6, this, "todo"
	).centerOn(900, 535);
}

void GraphicTeamManager::changePlayerAt(int position){

}

void GraphicTeamManager::onPlayersLoad()
{
	displayPlayers();
	_wait = false;
}
