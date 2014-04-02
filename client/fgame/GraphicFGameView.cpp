#include "GraphicFGameView.hpp"
#include <match/GraphicMatchView.hpp>

using namespace std;
using namespace GUI;
///Constructor
GraphicFGameView::GraphicFGameView(ClientController const & parent, GUI::MainController &controller)
		: FGameController(parent), GraphicManager(controller), _lastUpdated(0)
{
	askConnectedList();
	_canvas.setBackgroundImage("HexBack.png");

	displayCanvas();
	readMessages();
}

/**
 * Method handling player invitation to game
 * @param string: the player whom to send the invite
*/
void GraphicFGameView::invitePlayer(string playername)
{
	sendInvitation(playername);
}

/**
 * Method displaying connected users list
 * @param : JSON::List list of connected players
 */
void GraphicFGameView::onUserList(JSON::List const & list)
{
	_lastUpdated = time(NULL);
	clear();
	addTopBar(user());

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
			Button<GraphicFGameView, string> & inviteButton = current.addButton<GraphicFGameView, string>(
				&GraphicFGameView::invitePlayer, STR(list[i]).value(), this, "Invite");
			inviteButton.setPosition(350-inviteButton.getWidth(), 0);
		}
	}
	else{
		Label &noPlayersLabel = _canvas.addLabel("No one is connected. You are all alone.");
		noPlayersLabel.setPosition(500, 350);
		noPlayersLabel.setColor(sf::Color::White);
	}
	addBackButton();

	redrawCanvas();
}

void GraphicFGameView::onLoop(){
	if (abs(difftime(time(NULL), _lastUpdated)) > GUI::AUTOUPDATE_STALE_TIME)
		askConnectedList();
}
