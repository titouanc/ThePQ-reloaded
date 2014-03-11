#include "GUIFGameManager.hpp"

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

void GUIFGameManager::onUserList(JSON::List const & list){
	_canvas.clear();

	TableView & playersTV = _canvas.addTableView(2);
	playersTV.setPosition(50, 50);
	for (size_t i=0; i<list.len(); i++){
		TableCell & current = playersTV.addTableCell(350, BUTTON_HEIGHT);
		current.setBackgroundColor(sf::Color(0x00, 0x00, 0x00, 0x77));
		Label & nameLabel = current.addLabel(STR(list[i]).value());
		nameLabel.setPosition(15, 7);
		nameLabel.setColor(sf::Color(0xee, 0xee, 0xee, 0xff));
		current.addButton<GUIFGameManager, string>(
			&GUIFGameManager::invitePlayer, STR(list[i]).value(), this, "Invite"
		).setPosition(275, 0);
	}

	backButton().setPosition(900, 470);

	redrawCanvas();

}

void GUIFGameManager::invitePlayer(string playername){
	cout << playername << endl;
}
