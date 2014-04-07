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

/**
 * Method incresing the velocity of a player
 * @param int : id of the player whose velocity to be upgraded
 */
void GraphicTeamView::incVelocity(int player){
	user().players[player].improveVelocity(1);
	upgradePlayerAbility(user().players[player].getMemberID(),1);
	onPlayersLoad();
	onTeamInfoChange();
	redrawCanvas();
}

/**
 * Method incresing the strength of a player
 * @param int : id of the player whose strength to be upgraded
 */
void GraphicTeamView::incStrength(int  player){
	user().players[player].improveStrength(1);
	upgradePlayerAbility(user().players[player].getMemberID(),0);
	onPlayersLoad();
	onTeamInfoChange();
	redrawCanvas();
}

/**
 * Method incresing the chance of a player
 * @param int : id of the player whose chance to be upgraded
 */
void GraphicTeamView::incChance(int  player){
	user().players[player].improveChance(1);
	upgradePlayerAbility(user().players[player].getMemberID(),3);
	onPlayersLoad();
	onTeamInfoChange();
	redrawCanvas();
}

/**
 * Method incresing the precision of a player
 * @param int : id of the player whose precision to be upgraded
 */
void GraphicTeamView::incPrecision(int player){
	user().players[player].improvePrecision(1);
	upgradePlayerAbility(user().players[player].getMemberID(),2);
	onPlayersLoad();
	onTeamInfoChange();
	redrawCanvas();
}

/// Destructor
GraphicTeamView::~GraphicTeamView(){}

/**
 * Method displaying players and their abilities
 */
void GraphicTeamView::displayPlayers()
{
	clear();
	addTopBar(user());
	TableView & playerList = _canvas.addTableView();
	playerList.setPosition(30, 100);
	playerList.setElementsNumberPerPage(9);
	/* Header line */
	TableCell & header = playerList.addHeader(715, 47);
	header.addLabel("Name", sf::Color::White).setPosition(15, 10);
	header.addLabel("Strength", sf::Color::White).setPosition(245, 10);
	header.addLabel("Velocity", sf::Color::White).setPosition(382, 10);
	header.addLabel("Precision",sf::Color::White).setPosition(495,10);
	header.addLabel("Chance",sf::Color::White).setPosition(595,10);
	header.setBackgroundColor(BUTTON_BACKGROUND_COLOR);
	int acPoints=user().acPoints;
	/* Content */
	int height = 170,cter=0;
	for (Player & player : user().players){
		TableCell & playerCell = playerList.addTableCell(715, 47, sf::Color(0xe0, 0xe0, 0xe0, 0xff));
		playerCell.setBackgroundColor(sf::Color(0x00, 0x00, 0x00, 0x77));
		playerCell.addLabel(player.getName(), sf::Color::White
			).setPosition(25, 10);
		playerCell.addLabel(std::to_string(player.getStrength()), sf::Color::White
			).setPosition(245, 10);
		playerCell.addLabel(std::to_string(player.getStrength()+1), sf::Color::Yellow
			).setPosition(280, 10);
		if(acPoints>player.getStrength()+1 && player.getStrength()<100)	drawUpgradeButton(350,height,cter,0);
		playerCell.addLabel(std::to_string(player.getVelocity()), sf::Color::White
			).setPosition(382, 10);
		playerCell.addLabel(std::to_string(player.getVelocity()+1), sf::Color::Yellow
			).setPosition(417, 10);
		if(acPoints>player.getVelocity()+1 && player.getVelocity()<100)	drawUpgradeButton(487,height,cter,1);
		playerCell.addLabel(std::to_string(player.getPrecision()), sf::Color::White
			).setPosition(495,10);
		playerCell.addLabel(std::to_string(player.getPrecision()+1), sf::Color::Yellow
			).setPosition(528, 10);
		if(acPoints>player.getPrecision()+1 && player.getPrecision()<100)	drawUpgradeButton(598,height,cter,2);
		playerCell.addLabel(std::to_string(player.getChance()), sf::Color::White
			).setPosition(595,10);
		playerCell.addLabel(std::to_string(player.getChance()+1), sf::Color::Yellow
			).setPosition(628, 10);
		if(acPoints>player.getChance()+1 && player.getChance()<100)	drawUpgradeButton(698,height,cter,3);
		height+=52;
		cter+=1;
	}

	displaySquadLabels();

	addBackButton();

	redrawCanvas();
}

/**
 * Method handling the display of the upgrade buttons for the abilities
 * @param int : x coordinate for the button
 * @param int : y coordinate for the button
 * @param int : ability to be controlled by the button
 */
void GraphicTeamView::drawUpgradeButton(int x, int y,int player,int ability){
	switch (ability){
	case 0: {
		Button<GraphicTeamView> &strUp = _canvas.addButton<GraphicTeamView>(
			&GraphicTeamView::incStrength,player, this, "");
		strUp.setPosition(x, y);
		strUp.setWidth(15);
		strUp.setHeight(15);
		strUp.setBackgroundColor(sf::Color::Red);
		break;}
	case 1: {
		Button<GraphicTeamView> &strUp1 = _canvas.addButton<GraphicTeamView>(
			&GraphicTeamView::incVelocity,player, this, "");
		strUp1.setPosition(x, y);
		strUp1.setWidth(15);
		strUp1.setHeight(15);
		strUp1.setBackgroundColor(sf::Color::Red);
		break;}
	case 2: {
		Button<GraphicTeamView> &strUp2 = _canvas.addButton<GraphicTeamView>(
			&GraphicTeamView::incPrecision,player, this, "");
		strUp2.setPosition(x, y);
		strUp2.setWidth(15);
		strUp2.setHeight(15);
		strUp2.setBackgroundColor(sf::Color::Red);
		break;}
	case 3: {
		Button<GraphicTeamView> &strUp3 = _canvas.addButton<GraphicTeamView>(
			&GraphicTeamView::incChance,player, this, "");
		strUp3.setPosition(x, y);
		strUp3.setWidth(15);
		strUp3.setHeight(15);
		strUp3.setBackgroundColor(sf::Color::Red);
		break;}
	default:
		break;
	}
	
}

/**
 * Method handling the display of the squad labels
 */
void GraphicTeamView::displaySquadLabels(){
	_canvas.addPanel(515, 500, sf::Color(0x00, 0x00, 0x00, 0x77)
		).setPosition(750, 100);

	_canvas.addLabel("Your Squad", BLUE_TEXT_COLOR
		).centerOn(1000, 120);

	_canvas.addLabel("Left Chaser", sf::Color::White
		).centerOn(855, 160);
	_canvas.addLabel("Center Chaser", sf::Color::White
		).centerOn(1000, 250);
	_canvas.addLabel("Right Chaser", sf::Color::White
		).centerOn(1145, 160);

	_canvas.addLabel("Left Beater", sf::Color::White
		).centerOn(900, 340);
	_canvas.addLabel("Right Beater", sf::Color::White
		).centerOn(1099, 340);

	_canvas.addLabel("Seeker", sf::Color::White
		).centerOn(1000, 430);
	_canvas.addLabel("Keeper", sf::Color::White
		).centerOn(1000, 520);
}

/**
 * Method handling the display of the modifier buttons
 */
void GraphicTeamView::displayChangeButtons(){
	_canvas.addButton<GraphicTeamView, int>(
		&GraphicTeamView::changePlayerAt, LEFT_CHASER, this, user().squad.players[LEFT_CHASER]->getName()
	).centerOn(855, 205);
	_canvas.addButton<GraphicTeamView, int>(
		&GraphicTeamView::changePlayerAt, CENTER_CHASER, this, user().squad.players[CENTER_CHASER]->getName()
	).centerOn(1000, 295);
	_canvas.addButton<GraphicTeamView, int>(
		&GraphicTeamView::changePlayerAt, RIGHT_CHASER, this, user().squad.players[RIGHT_CHASER]->getName()
	).centerOn(1145, 205);

	_canvas.addButton<GraphicTeamView, int>(
		&GraphicTeamView::changePlayerAt, LEFT_BEATER, this, user().squad.players[LEFT_BEATER]->getName()
	).centerOn(900, 385);
	_canvas.addButton<GraphicTeamView, int>(
		&GraphicTeamView::changePlayerAt, RIGHT_BEATER, this, user().squad.players[RIGHT_BEATER]->getName()
	).centerOn(1099, 385);

	_canvas.addButton<GraphicTeamView, int>(
		&GraphicTeamView::changePlayerAt, SEEKER, this, user().squad.players[SEEKER]->getName()
	).centerOn(1000, 475);
	_canvas.addButton<GraphicTeamView, int>(
		&GraphicTeamView::changePlayerAt, KEEPER, this, user().squad.players[KEEPER]->getName()
	).centerOn(1000, 565);
	redrawCanvas();
}

/**
 * Method handling the player swap
 * @param int : position of the player to be swapped
 */
void GraphicTeamView::changePlayerAt(int position){
	selectPosition(position); // storing it in an instance variable
	displayPlayers();
	displayAvailablePlayers();
	displaySwapWith(position);
}

/**
 * Method handling the display of the swap interface
 * @param int : position of the player to be swapped
 */
void GraphicTeamView::displaySwapWith(int position){
	if (LEFT_CHASER != position)
		_canvas.addButton<GraphicTeamView, int>(
			&GraphicTeamView::doSwap, user().squad.players[LEFT_CHASER]->getMemberID(), this, "Swap"
		).centerOn(855, 205);
	else
		_canvas.addButton<GraphicTeamView>(
			&GraphicTeamView::cancelChange, this, "Cancel"
		).centerOn(855, 205);
	if (CENTER_CHASER != position)
		_canvas.addButton<GraphicTeamView, int>(
			&GraphicTeamView::doSwap, user().squad.players[CENTER_CHASER]->getMemberID(), this, "Swap"
		).centerOn(1000, 295);
	else
		_canvas.addButton<GraphicTeamView>(
			&GraphicTeamView::cancelChange, this, "Cancel"
		).centerOn(1000, 295);
	if (RIGHT_CHASER != position)
		_canvas.addButton<GraphicTeamView, int>(
			&GraphicTeamView::doSwap, user().squad.players[RIGHT_CHASER]->getMemberID(), this, "Swap"
		).centerOn(1145, 205);
	else
		_canvas.addButton<GraphicTeamView>(
			&GraphicTeamView::cancelChange, this, "Cancel"
		).centerOn(1145, 205);

	if (LEFT_BEATER != position)
		_canvas.addButton<GraphicTeamView, int>(
			&GraphicTeamView::doSwap, user().squad.players[LEFT_BEATER]->getMemberID(), this, "Swap"
		).centerOn(900, 385);
	else
		_canvas.addButton<GraphicTeamView>(
			&GraphicTeamView::cancelChange, this, "Cancel"
		).centerOn(900, 385);
	if (RIGHT_BEATER != position)
		_canvas.addButton<GraphicTeamView, int>(
			&GraphicTeamView::doSwap, user().squad.players[RIGHT_BEATER]->getMemberID(), this, "Swap"
		).centerOn(1099, 385);
	else
		_canvas.addButton<GraphicTeamView>(
			&GraphicTeamView::cancelChange, this, "Cancel"
		).centerOn(1099, 385);

	if (SEEKER != position)
		_canvas.addButton<GraphicTeamView, int>(
			&GraphicTeamView::doSwap, user().squad.players[SEEKER]->getMemberID(), this, "Swap"
		).centerOn(1000, 475);
	else
		_canvas.addButton<GraphicTeamView>(
			&GraphicTeamView::cancelChange, this, "Cancel"
		).centerOn(1000, 475);
	if (KEEPER != position)
		_canvas.addButton<GraphicTeamView, int>(
			&GraphicTeamView::doSwap, user().squad.players[KEEPER]->getMemberID(), this, "Swap"
		).centerOn(1000, 565);
	else
		_canvas.addButton<GraphicTeamView>(
			&GraphicTeamView::cancelChange, this, "Cancel"
		).centerOn(1000, 565);
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
 * Method handling the swap of players old position <->new position
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
