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

	displaySquad();

	backButton();

	redrawCanvas();
}

void GraphicTeamManager::displaySquad(){
	_canvas.addPanel(515, 500, sf::Color(0x00, 0x00, 0x00, 0x77)
		).setPosition(650, 100);

	_canvas.addLabel("Left Chaser", sf::Color::White
		).setPosition(680, 160);
	_canvas.addLabel("Center Chaser", sf::Color::White
		).setPosition(835, 160);
	_canvas.addLabel("Right Chaser", sf::Color::White
		).setPosition(1010, 160);

	_canvas.addLabel("Left Beater", sf::Color::White
		).setPosition(750, 265);
	_canvas.addLabel("Right Beater", sf::Color::White
		).setPosition(950, 265);

	_canvas.addLabel("Seeker", sf::Color::White
		).setPosition(865, 370);
	_canvas.addLabel("Keeper", sf::Color::White
		).setPosition(865, 485);

}

void GraphicTeamManager::onPlayersLoad()
{
	displayPlayers();
	_wait = false;
	cout << "GOT HERE" << endl;
}
