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

void GUIFGameManager::loop()
{
	readMessages();
	handleNotification();
}

void GUIFGameManager::onUserList(JSON::List const & list)
{
	_canvas.clear();

	TableView & playersTV = _canvas.addTableView(2);
	playersTV.setPosition(50, 50);
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

	backButton().setPosition(900, 450);

	redrawCanvas();
}

void GUIFGameManager::launchMatch()
{
	GraphicMatchManager match(*this, _controller);
	match.run();
	redrawCanvas();
}

void GUIFGameManager::onOtherAccept(std::string const & user)
{
	launchMatch();
}

void GUIFGameManager::onOtherDeny(std::string const & user)
{
	displayError(user + " doesn't want to play with you");
}

void GUIFGameManager::onInvite(std::string const & otherUser)
{
	std::string question = otherUser + " has invited you to play a friendly game.\nAccept ?";
	if (confirm(question)){
		acceptInvitationFromUser(otherUser);
		launchMatch();
	} else {
		denyInvitationFromUser(otherUser);
	}
}
