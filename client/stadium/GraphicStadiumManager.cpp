#include "GraphicStadiumManager.hpp"
#include <match/GraphicMatchManager.hpp>

using namespace std;
using namespace GUI;

GraphicStadiumManager::GraphicStadiumManager(
	ClientManager const & parent, 
	GUI::MainController &controller
) : 
	StadiumManager(parent), 
	GraphicManager(controller)
{
	_canvas.setBackgroundImage(texturePath("HexBack.png"));
	displayCanvas();
	loadInstallations();
	_wait = true;
	while (_wait){
		readMessages();
		readEvent();
	}
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

	backButton().setPosition(900, 650);

	redrawCanvas();
}

void GraphicStadiumManager::onInstallationsLoad()
{
	_wait = false;
	displayInstallations();
}
