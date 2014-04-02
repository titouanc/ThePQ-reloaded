#include "GraphicStadiumView.hpp"
#include <match/GraphicMatchView.hpp>

using namespace std;
using namespace GUI;

///Constructor
GraphicStadiumView::GraphicStadiumView(
			ClientController const & parent, 
			GUI::MainController &controller
	) : 
	StadiumController(parent), 
	GraphicManager(controller)
{
	_canvas.setBackgroundImage("HexBack.png");
	displayCanvas();
	loadInstallations();
	while (_wait){
		readMessages();
		readEvent();
	}
}

/**
 * Method handling installation interface
 */
void GraphicStadiumView::displayInstallations()
{
	
	readMessages();

	clear();

	addTopBar(user());

	TableView & installations = _canvas.addTableView(2, 10);
	installations.setPosition(85, 85);

	for (size_t i = 0; i < user().installations.size(); ++i){
		TableCell & current = installations.addTableCell(270, 270, sf::Color(0x00, 0x00, 0x00, 0x77));
		Label &nameLabel = current.addLabel(user().installations[i]->getName());
		nameLabel.setPosition(10, 10);
		//nameLabel.setFont(HEADER_FONT_PATH);
		nameLabel.setColor(BLUE_TEXT_COLOR);
		
		Label &levelLabel = current.addLabel(user().installations[i]->getLevel());	
		levelLabel.setPosition(270-15-levelLabel.getWidth(), 10);
		levelLabel.setColor(BLUE_TEXT_COLOR);
		//levelLabel.setFont(HEADER_FONT_PATH);

		current.addLabel("Upgrade cost : ", sf::Color::White).setPosition(10, 50);
		Label &upgradeCostValue = current.addLabel(user().installations[i]->getUpgradeCost());	
		upgradeCostValue.setPosition(270-15-upgradeCostValue.getWidth(), 50);
		upgradeCostValue.setColor(BLUE_TEXT_COLOR);

		current.addLabel("Downgrade refunds : ", sf::Color::White).setPosition(10, 90);
		Label &downgradeRefundsValue = current.addLabel(user().installations[i]->getDowngradeRefunds());	
		downgradeRefundsValue.setPosition(270-15-downgradeRefundsValue.getWidth(), 90);
		downgradeRefundsValue.setColor(BLUE_TEXT_COLOR);

		if (user().installations[i]->getLevel() == 0){
			current.addButton<GraphicStadiumView, int>(
				&GraphicStadiumView::doUpgrade, i, this, "Buy", GREEN_BUTTON_BACKGROUND_COLOR
			).setPosition(10, 220);
		}
		else{
			current.addButton<GraphicStadiumView, int>(
				&GraphicStadiumView::doUpgrade, i, this, "Upgrade", GREEN_BUTTON_BACKGROUND_COLOR
			).setPosition(10, 220);

			Button<GraphicStadiumView, int> &downButton = current.addButton<GraphicStadiumView, int>(
				&GraphicStadiumView::doDowngrade, i, this, "Downgrade",RED_BUTTON_BACKGROUND_COLOR);
			downButton.setPosition(270-10-downButton.getWidth(), 220);
		}
	}

	Label &titleLabel = _canvas.addLabel("Your\ninstallations");
	titleLabel.setFont(HEADER_FONT_PATH);
	titleLabel.setColor(sf::Color::White);
	titleLabel.setFontSize(40);
	titleLabel.setPosition(700, 300);

	addBackButton();

	redrawCanvas();
}

/**
 * Method handling upgrades of an installation
 * @param int : id of the installation
 */
void GraphicStadiumView::doUpgrade(int installation){
	upgradeInstallation(installation);
	_wait = true;
	while (_wait) 
		readMessage();
}

/**
 * Method handling the downgrade of an installations
 * @param int : id of the installation
 */
void GraphicStadiumView::doDowngrade(int installation){
	downgradeInstallation(installation);
	_wait = true;
	while (_wait)
		readMessage();
}

/**
 * Method handling the interface for the installations
 */
void GraphicStadiumView::onInstallationsLoad()
{
	_wait = false;
	displayInstallations();
}
