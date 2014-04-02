#include "GraphicTeamView.hpp"
#include <sstream>

using namespace std;
using namespace GUI;
using namespace SQUAD;

/// Constructor
GraphicTeamView::GraphicTeamView(
		ClientController const & parent, 
		GUI::MainController &controller
	) : 
		TeamController(parent), 
		GraphicManager(controller)
{
	_canvas.setBackgroundImage("HexBack.png");
	displayCanvas();
	if (user().players.size() > 0){
		displayPlayers();
		displayChangeButtons();
	} else {
		loadPlayers();
		while (_wait){
			readMessages();
			readEvent();
		}
	}
}

/// Destructor
GraphicTeamView::~GraphicTeamView(){}

/**
 * Method
 */
void GraphicTeamView::displayPlayers()
{
	clear();

	addTopBar(user());
	//_canvas.addLabel()
	TableView & playerList = _canvas.addTableView();
	playerList.setPosition(30, 100);
	playerList.setElementsNumberPerPage(9);
	/* Header line */
	TableCell & header = playerList.addHeader(575, 47);
	header.addLabel("Name", sf::Color::White).setPosition(15, 10);
	header.addLabel("Strength", sf::Color::White).setPosition(245, 10);
	header.addLabel("Velocity", sf::Color::White).setPosition(332, 10);
	header.addLabel("Precision",sf::Color::White).setPosition(410,10);
	header.addLabel("Chance",sf::Color::White).setPosition(495,10);
	header.setBackgroundColor(BUTTON_BACKGROUND_COLOR);

	/* Content */
	for (Player & player : user().players){
		TableCell & playerCell = playerList.addTableCell(575, 47, sf::Color(0xee, 0xee, 0xee, 0xff));
		playerCell.setBackgroundColor(sf::Color(0x00, 0x00, 0x00, 0x77));
		playerCell.addLabel(player.getName(), sf::Color::White).setPosition(25, 10);;
		playerCell.addLabel(player.getStrength(), sf::Color::White
			).setPosition(270, 10);
		playerCell.addLabel(player.getVelocity(), sf::Color::White
			).setPosition(356, 10);
		playerCell.addLabel(player.getPrecision(), sf::Color::White).setPosition(445,10);
		playerCell.addLabel(player.getChance(), sf::Color::White).setPosition(520,10);
	}

	displaySquadLabels();

	addBackButton();

	redrawCanvas();
}

/**
 * Method
 */
void GraphicTeamView::displaySquadLabels(){
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

/**
 * Method
 */
void GraphicTeamView::displayChangeButtons(){
	_canvas.addButton<GraphicTeamView, int>(
		&GraphicTeamView::changePlayerAt, LEFT_CHASER, this, user().squad.players[LEFT_CHASER]->getName()
	).centerOn(755, 205);
	_canvas.addButton<GraphicTeamView, int>(
		&GraphicTeamView::changePlayerAt, CENTER_CHASER, this, user().squad.players[CENTER_CHASER]->getName()
	).centerOn(900, 295);
	_canvas.addButton<GraphicTeamView, int>(
		&GraphicTeamView::changePlayerAt, RIGHT_CHASER, this, user().squad.players[RIGHT_CHASER]->getName()
	).centerOn(1045, 205);

	_canvas.addButton<GraphicTeamView, int>(
		&GraphicTeamView::changePlayerAt, LEFT_BEATER, this, user().squad.players[LEFT_BEATER]->getName()
	).centerOn(800, 385);
	_canvas.addButton<GraphicTeamView, int>(
		&GraphicTeamView::changePlayerAt, RIGHT_BEATER, this, user().squad.players[RIGHT_BEATER]->getName()
	).centerOn(999, 385);

	_canvas.addButton<GraphicTeamView, int>(
		&GraphicTeamView::changePlayerAt, SEEKER, this, user().squad.players[SEEKER]->getName()
	).centerOn(900, 475);
	_canvas.addButton<GraphicTeamView, int>(
		&GraphicTeamView::changePlayerAt, KEEPER, this, user().squad.players[KEEPER]->getName()
	).centerOn(900, 565);

	redrawCanvas();
}

/**
 * Method
 */
void GraphicTeamView::changePlayerAt(int position){
	selectPosition(position); // storing it in an instance variable
	displayPlayers();
	displayAvailablePlayers();
	displaySwapWith(position);
}

/**
 * Method
 */
void GraphicTeamView::displaySwapWith(int position){
	if (LEFT_CHASER != position)
		_canvas.addButton<GraphicTeamView, int>(
			&GraphicTeamView::doSwap, user().squad.players[LEFT_CHASER]->getMemberID(), this, "Swap"
		).centerOn(755, 205);
	else
		_canvas.addButton<GraphicTeamView>(
			&GraphicTeamView::cancelChange, this, "Cancel"
		).centerOn(755, 205);
	if (CENTER_CHASER != position)
		_canvas.addButton<GraphicTeamView, int>(
			&GraphicTeamView::doSwap, user().squad.players[CENTER_CHASER]->getMemberID(), this, "Swap"
		).centerOn(900, 295);
	else
		_canvas.addButton<GraphicTeamView>(
			&GraphicTeamView::cancelChange, this, "Cancel"
		).centerOn(900, 295);
	if (RIGHT_CHASER != position)
		_canvas.addButton<GraphicTeamView, int>(
			&GraphicTeamView::doSwap, user().squad.players[RIGHT_CHASER]->getMemberID(), this, "Swap"
		).centerOn(1045, 205);
	else
		_canvas.addButton<GraphicTeamView>(
			&GraphicTeamView::cancelChange, this, "Cancel"
		).centerOn(1045, 205);

	if (LEFT_BEATER != position)
		_canvas.addButton<GraphicTeamView, int>(
			&GraphicTeamView::doSwap, user().squad.players[LEFT_BEATER]->getMemberID(), this, "Swap"
		).centerOn(800, 385);
	else
		_canvas.addButton<GraphicTeamView>(
			&GraphicTeamView::cancelChange, this, "Cancel"
		).centerOn(800, 385);
	if (RIGHT_BEATER != position)
		_canvas.addButton<GraphicTeamView, int>(
			&GraphicTeamView::doSwap, user().squad.players[RIGHT_BEATER]->getMemberID(), this, "Swap"
		).centerOn(999, 385);
	else
		_canvas.addButton<GraphicTeamView>(
			&GraphicTeamView::cancelChange, this, "Cancel"
		).centerOn(999, 385);

	if (SEEKER != position)
		_canvas.addButton<GraphicTeamView, int>(
			&GraphicTeamView::doSwap, user().squad.players[SEEKER]->getMemberID(), this, "Swap"
		).centerOn(900, 475);
	else
		_canvas.addButton<GraphicTeamView>(
			&GraphicTeamView::cancelChange, this, "Cancel"
		).centerOn(900, 475);
	if (KEEPER != position)
		_canvas.addButton<GraphicTeamView, int>(
			&GraphicTeamView::doSwap, user().squad.players[KEEPER]->getMemberID(), this, "Swap"
		).centerOn(900, 565);
	else
		_canvas.addButton<GraphicTeamView>(
			&GraphicTeamView::cancelChange, this, "Cancel"
		).centerOn(900, 565);

	redrawCanvas();
}

/**
 * Method displaying the available players on the interface
 */
void GraphicTeamView::displayAvailablePlayers(){
	for(size_t i=0; i<user().players.size(); ++i){
		if (!user().squad.hasMemberID(user().players[i].getMemberID())){
			_canvas.addButton<GraphicTeamView, int>(
				&GraphicTeamView::selectPlayer, user().players[i].getMemberID(), this, "Select" 
				).setPosition(10, 153+i*52);
		}
	}
}

/**
 * Method handpling player positioning on the pitch
 */
void GraphicTeamView::selectPlayer(int id){
	putPlayerAtPosition(id, getSelectedPosition());
	_wait = true;
	while (_wait)
		readMessage();
}

/**
 * Method handling the fwap of players old position <->new position
 */
void GraphicTeamView::doSwap(int id){
	swapPlayers(id, getSelectedPosition());
	_wait = true;
	while (_wait)
		readMessage();
}

/**
 * Method reversing user chages
 */
void GraphicTeamView::cancelChange(){
	displayPlayers();
	displayChangeButtons();
}

/**
 * Method handling player interface
 */
void GraphicTeamView::onPlayersLoad()
{
	displayPlayers();
	displayChangeButtons();
	_wait = false;
}

/**
 * Method handling squad interface
 */
void GraphicTeamView::onSquadUpdated()
{
	_wait = false;
	displayPlayers();
	displayChangeButtons();
}
