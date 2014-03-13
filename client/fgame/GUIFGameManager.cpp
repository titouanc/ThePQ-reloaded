#include "GUIFGameManager.hpp"
#include <match/GraphicMatchManager.hpp>

using namespace std;
using namespace GUI;

GUIFGameManager::GUIFGameManager(ClientManager const & parent, GUI::MainController &controller)
		: FGameManager(parent), GraphicManager(controller)
{
	askConnectedList();

	_canvas.setBackgroundImage(texturePath("HexBack.png"));

	displayCanvas();
	readMessages();
}

void GUIFGameManager::invitePlayer(string playername)
{
	sendInvitation(playername);
	Button<GUIFGameManager, string> * inviteButton = inviteButtons[playername];
	inviteButton->disable();
	inviteButton->setBackgroundColor(sf::Color(0xcc, 0xcc, 0xcc, 0xff));
	inviteButton->setText("Invited");
	inviteButton->setPosition(350-inviteButton->getWidth(), 0);
	redrawCanvas();
}

void GUIFGameManager::onUserList(JSON::List const & list)
{
	_canvas.clear();

	usernameButton(user().username);
	userBudgetButton(user().funds);
	userAcPointsButton(user().acPoints);
	userFameButton(user().fame);

	Label &titleLabel = _canvas.addLabel("Connected users");
	titleLabel.setFont(HEADER_FONT_PATH);
	titleLabel.setFontSize(40);
	titleLabel.setPosition(85, 85);
	titleLabel.setColor(sf::Color::White);

	if (list.len() != 0){
		TableView & playersTV = _canvas.addTableView(2);
		playersTV.setPosition(100, 150);

		for (size_t i=0; i<list.len(); i++){
			TableCell & current = playersTV.addTableCell(350, BUTTON_HEIGHT);
			current.setBackgroundColor(sf::Color(0x00, 0x00, 0x00, 0x77));
			Label & nameLabel = current.addLabel(STR(list[i]).value());
			nameLabel.setPosition(15, 7);
			nameLabel.setColor(sf::Color(0xee, 0xee, 0xee, 0xff));
			Button<GUIFGameManager, string> & inviteButton = current.addButton<GUIFGameManager, string>(
				&GUIFGameManager::invitePlayer, STR(list[i]).value(), this, "Invite");
			inviteButton.setPosition(350-inviteButton.getWidth(), 0);
			inviteButtons.insert(pair<string, GUI::Button<GUIFGameManager, string>*>(STR(list[i]).value(), &inviteButton));
		}
	}
	else{
		Label &noPlayersLabel = _canvas.addLabel("No one is connected. You are all alone.");
		noPlayersLabel.setPosition(500, 350);
		noPlayersLabel.setColor(sf::Color::White);
	}

	backButton();

	redrawCanvas();
}
