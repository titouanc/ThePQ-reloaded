#include "GUIFGameManager.hpp"

using namespace std;
using namespace GUI;

GUIFGameManager::GUIFGameManager(ClientManager const & parent, GUI::MainController &controller)
		: FGameManager(parent), GraphicManager(controller)
{
	askConnectedList();
	displayCanvas();
	readMessages();
}

void GUIFGameManager::onUserList(JSON::List const & list){
	_canvas.clear();

	TableView & playersTV = _canvas.addTableView(2);
	for (size_t i=0; i<list.len(); i++){
		TableCell & current = playersTV.addTableCell(300, 47);
		current.setBackgroundColor(sf::Color(0x00, 0x00, 0x00, 0x99));
		current.addLabel(STR(list[i]).value()).setPosition(15, 10);
		current.addButton<GUIFGameManager, string>(
			&GUIFGameManager::invitePlayer, STR(list[i]).value(), this, "Invite"
		).setPosition(240, 2);
	}

}

void GUIFGameManager::invitePlayer(string playername){
	cout << playername << endl;
}
