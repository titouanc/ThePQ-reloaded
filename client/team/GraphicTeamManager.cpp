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

	TableView & playerList = _canvas.addTableView();
	/* Header line */
	TableCell & header = playerList.addTableCell(515, 47);
	header.addLabel("Name").setPosition(15, 10);
	header.addLabel("Life").setPosition(355, 10);
	header.addLabel("Mana").setPosition(435, 10);

	/* Content */
	for (Player & player : user().players){
		TableCell & playerCell = playerList.addTableCell(500, 47, sf::Color(0xee, 0xee, 0xee, 0xff));
		playerCell.addLabel(player.getName()).setPosition(15, 10);;
		Label & lifeLabel = playerCell.addLabel(player.getRemainingLife());
		lifeLabel.setPosition(355, 10);
		lifeLabel.setColor(GREEN_TEXT_COLOR);
		Label & manaLabel = playerCell.addLabel(player.getRemainingMana());
		manaLabel.setPosition(435, 10);
		manaLabel.setColor(BLUE_TEXT_COLOR);
	}

	backButton();

	redrawCanvas();
}

void GraphicTeamManager::onPlayersLoad()
{
	displayPlayers();
	_wait = false;
	cout << "GOT HERE" << endl;
}
