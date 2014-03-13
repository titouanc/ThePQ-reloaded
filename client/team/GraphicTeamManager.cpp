#include "GraphicTeamManager.hpp"

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
		&GraphicTeamManager::changePlayerAt, LEFT_CHASER, this, user().squad.players[LEFT_CHASER]->getName()
	).centerOn(755, 210);
	_canvas.addButton<GraphicTeamManager, int>(
		&GraphicTeamManager::changePlayerAt, CENTER_CHASER, this, user().squad.players[CENTER_CHASER]->getName()
	).centerOn(900, 210);
	_canvas.addButton<GraphicTeamManager, int>(
		&GraphicTeamManager::changePlayerAt, RIGHT_CHASER, this, user().squad.players[RIGHT_CHASER]->getName()
	).centerOn(1045, 210);

	_canvas.addButton<GraphicTeamManager, int>(
		&GraphicTeamManager::changePlayerAt, LEFT_BEATER, this, user().squad.players[LEFT_BEATER]->getName()
	).centerOn(816, 315);
	_canvas.addButton<GraphicTeamManager, int>(
		&GraphicTeamManager::changePlayerAt, RIGHT_BEATER, this, user().squad.players[RIGHT_BEATER]->getName()
	).centerOn(983, 315);

	_canvas.addButton<GraphicTeamManager, int>(
		&GraphicTeamManager::changePlayerAt, SEEKER, this, user().squad.players[SEEKER]->getName()
	).centerOn(900, 420);
	_canvas.addButton<GraphicTeamManager, int>(
		&GraphicTeamManager::changePlayerAt, KEEPER, this, user().squad.players[KEEPER]->getName()
	).centerOn(900, 535);

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
			&GraphicTeamManager::changePlayerAt, LEFT_CHASER, this, "Swap"
		).centerOn(755, 210);
	else
		_canvas.addButton<GraphicTeamManager, int>(
			&GraphicTeamManager::changePlayerAt, LEFT_CHASER, this, "Keep"
		).centerOn(755, 210);
	if (CENTER_CHASER != position)
		_canvas.addButton<GraphicTeamManager, int>(
			&GraphicTeamManager::changePlayerAt, CENTER_CHASER, this, "Swap"
		).centerOn(900, 210);
	else
		_canvas.addButton<GraphicTeamManager, int>(
			&GraphicTeamManager::changePlayerAt, CENTER_CHASER, this, "Keep"
		).centerOn(900, 210);
	if (RIGHT_CHASER != position)
		_canvas.addButton<GraphicTeamManager, int>(
			&GraphicTeamManager::changePlayerAt, RIGHT_CHASER, this, "Swap"
		).centerOn(1045, 210);
	else
		_canvas.addButton<GraphicTeamManager, int>(
			&GraphicTeamManager::changePlayerAt, RIGHT_CHASER, this, "Keep"
		).centerOn(1045, 210);

	if (LEFT_BEATER != position)
		_canvas.addButton<GraphicTeamManager, int>(
			&GraphicTeamManager::changePlayerAt, LEFT_BEATER, this, "Swap"
		).centerOn(816, 315);
	else
		_canvas.addButton<GraphicTeamManager, int>(
			&GraphicTeamManager::changePlayerAt, LEFT_BEATER, this, "Keep"
		).centerOn(816, 315);
	if (RIGHT_BEATER != position)
		_canvas.addButton<GraphicTeamManager, int>(
			&GraphicTeamManager::changePlayerAt, RIGHT_BEATER, this, "Swap"
		).centerOn(983, 315);
	else
		_canvas.addButton<GraphicTeamManager, int>(
			&GraphicTeamManager::changePlayerAt, RIGHT_BEATER, this, "Swap"
		).centerOn(983, 315);

	if (SEEKER != position)
		_canvas.addButton<GraphicTeamManager, int>(
			&GraphicTeamManager::changePlayerAt, SEEKER, this, "Swap"
		).centerOn(900, 420);
	else
		_canvas.addButton<GraphicTeamManager, int>(
			&GraphicTeamManager::changePlayerAt, SEEKER, this, "Keep"
		).centerOn(900, 420);
	if (KEEPER != position)
		_canvas.addButton<GraphicTeamManager, int>(
			&GraphicTeamManager::changePlayerAt, KEEPER, this, "Swap"
		).centerOn(900, 535);
	else
		_canvas.addButton<GraphicTeamManager, int>(
			&GraphicTeamManager::changePlayerAt, KEEPER, this, "Keep"
		).centerOn(900, 535);

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
	cout << "1" << endl;
	while (_wait);
		readMessages();
}

void GraphicTeamManager::onPlayersLoad()
{
	displayPlayers();
	displayChangeButtons();
	_wait = false;
}

void GraphicTeamManager::onTeamInfoChange()
{
	_wait = false;
	displayPlayers();
	displayChangeButtons();
}
