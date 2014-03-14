#include "GraphicTeamManager.hpp"
#include <sstream>

using namespace std;
using namespace GUI;
using namespace SQUAD;

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
		displayChangeButtons();
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
	clear();

	addTopBar(user());

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

	addBackButton();

	redrawCanvas();
}

void GraphicTeamManager::displaySquadLabels(){
	_canvas.addPanel(515, 500, sf::Color(0x00, 0x00, 0x00, 0x77)
		).setPosition(650, 100);

	_canvas.addLabel("Your Squad", BLUE_TEXT_COLOR
		).centerOn(900, 120);

	_canvas.addLabel("Left Chaser", sf::Color::White
		).centerOn(755, 160);
	_canvas.addLabel("Center Chaser", sf::Color::White
		).centerOn(900, 250);
	_canvas.addLabel("Right Chaser", sf::Color::White
		).centerOn(1045, 160);

	_canvas.addLabel("Left Beater", sf::Color::White
		).centerOn(800, 340);
	_canvas.addLabel("Right Beater", sf::Color::White
		).centerOn(999, 340);

	_canvas.addLabel("Seeker", sf::Color::White
		).centerOn(900, 430);
	_canvas.addLabel("Keeper", sf::Color::White
		).centerOn(900, 520);
}

void GraphicTeamManager::displayChangeButtons(){
	_canvas.addButton<GraphicTeamManager, int>(
		&GraphicTeamManager::changePlayerAt, LEFT_CHASER, this, user().squad.players[LEFT_CHASER]->getName()
	).centerOn(755, 205);
	_canvas.addButton<GraphicTeamManager, int>(
		&GraphicTeamManager::changePlayerAt, CENTER_CHASER, this, user().squad.players[CENTER_CHASER]->getName()
	).centerOn(900, 295);
	_canvas.addButton<GraphicTeamManager, int>(
		&GraphicTeamManager::changePlayerAt, RIGHT_CHASER, this, user().squad.players[RIGHT_CHASER]->getName()
	).centerOn(1045, 205);

	_canvas.addButton<GraphicTeamManager, int>(
		&GraphicTeamManager::changePlayerAt, LEFT_BEATER, this, user().squad.players[LEFT_BEATER]->getName()
	).centerOn(800, 385);
	_canvas.addButton<GraphicTeamManager, int>(
		&GraphicTeamManager::changePlayerAt, RIGHT_BEATER, this, user().squad.players[RIGHT_BEATER]->getName()
	).centerOn(999, 385);

	_canvas.addButton<GraphicTeamManager, int>(
		&GraphicTeamManager::changePlayerAt, SEEKER, this, user().squad.players[SEEKER]->getName()
	).centerOn(900, 475);
	_canvas.addButton<GraphicTeamManager, int>(
		&GraphicTeamManager::changePlayerAt, KEEPER, this, user().squad.players[KEEPER]->getName()
	).centerOn(900, 565);

	redrawCanvas();
}

void GraphicTeamManager::changePlayerAt(int position){
	selectPosition(position); // storing it in an instance variable
	displayPlayers();
	displayAvailablePlayers();
	displaySwapWith(position);
}

void GraphicTeamManager::displaySwapWith(int position){
	if (LEFT_CHASER != position)
		_canvas.addButton<GraphicTeamManager, int>(
			&GraphicTeamManager::doSwap, user().squad.players[LEFT_CHASER]->getMemberID(), this, "Swap"
		).centerOn(755, 205);
	else
		_canvas.addButton<GraphicTeamManager>(
			&GraphicTeamManager::cancelChange, this, "Cancel"
		).centerOn(755, 205);
	if (CENTER_CHASER != position)
		_canvas.addButton<GraphicTeamManager, int>(
			&GraphicTeamManager::doSwap, user().squad.players[CENTER_CHASER]->getMemberID(), this, "Swap"
		).centerOn(900, 295);
	else
		_canvas.addButton<GraphicTeamManager>(
			&GraphicTeamManager::cancelChange, this, "Cancel"
		).centerOn(900, 295);
	if (RIGHT_CHASER != position)
		_canvas.addButton<GraphicTeamManager, int>(
			&GraphicTeamManager::doSwap, user().squad.players[RIGHT_CHASER]->getMemberID(), this, "Swap"
		).centerOn(1045, 205);
	else
		_canvas.addButton<GraphicTeamManager>(
			&GraphicTeamManager::cancelChange, this, "Cancel"
		).centerOn(1045, 205);

	if (LEFT_BEATER != position)
		_canvas.addButton<GraphicTeamManager, int>(
			&GraphicTeamManager::doSwap, user().squad.players[LEFT_BEATER]->getMemberID(), this, "Swap"
		).centerOn(800, 385);
	else
		_canvas.addButton<GraphicTeamManager>(
			&GraphicTeamManager::cancelChange, this, "Cancel"
		).centerOn(800, 385);
	if (RIGHT_BEATER != position)
		_canvas.addButton<GraphicTeamManager, int>(
			&GraphicTeamManager::doSwap, user().squad.players[RIGHT_BEATER]->getMemberID(), this, "Swap"
		).centerOn(999, 385);
	else
		_canvas.addButton<GraphicTeamManager>(
			&GraphicTeamManager::cancelChange, this, "Cancel"
		).centerOn(999, 385);

	if (SEEKER != position)
		_canvas.addButton<GraphicTeamManager, int>(
			&GraphicTeamManager::doSwap, user().squad.players[SEEKER]->getMemberID(), this, "Swap"
		).centerOn(900, 475);
	else
		_canvas.addButton<GraphicTeamManager>(
			&GraphicTeamManager::cancelChange, this, "Cancel"
		).centerOn(900, 475);
	if (KEEPER != position)
		_canvas.addButton<GraphicTeamManager, int>(
			&GraphicTeamManager::doSwap, user().squad.players[KEEPER]->getMemberID(), this, "Swap"
		).centerOn(900, 565);
	else
		_canvas.addButton<GraphicTeamManager>(
			&GraphicTeamManager::cancelChange, this, "Cancel"
		).centerOn(900, 565);

	redrawCanvas();
}

void GraphicTeamManager::displayAvailablePlayers(){
	for(int i=0; i<user().players.size(); ++i){
		if (!user().squad.hasMemberID(user().players[i].getMemberID())){
			_canvas.addButton<GraphicTeamManager, int>(
				&GraphicTeamManager::selectPlayer, user().players[i].getMemberID(), this, "Select" 
				).setPosition(10, 153+i*52);
		}
	}
}

void GraphicTeamManager::selectPlayer(int id){
	putPlayerAtPosition(id, getSelectedPosition());
	_wait = true;
	while (_wait)
		readMessage();
}

void GraphicTeamManager::doSwap(int id){
	swapPlayers(id, getSelectedPosition());
	_wait = true;
	while (_wait)
		readMessage();
}

void GraphicTeamManager::cancelChange(){
	displayPlayers();
	displayChangeButtons();
}

void GraphicTeamManager::onPlayersLoad()
{
	displayPlayers();
	displayChangeButtons();
	_wait = false;
}

void GraphicTeamManager::onSquadUpdated()
{
	_wait = false;
	displayPlayers();
	displayChangeButtons();
}
