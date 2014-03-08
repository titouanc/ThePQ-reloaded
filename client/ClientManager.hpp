#ifndef DEFINE_CLIENTMANAGER_HEADER
#define DEFINE_CLIENTMANAGER_HEADER

#include <network/ClientConnectionManager.hpp>
#include "UserData.hpp"
#include <sstream>
#include <string>
#include <queue>

struct TeamInfo {
	std::string username, teamname;
	int funds;
	TeamInfo(JSON::Dict const & json);
};

class ClientManager {
	private:
		net::ClientConnectionManager & _connection;
		UserData & _user;
		std::queue<std::string> & _notifications;

	protected:
		/* Return connection object */
		net::ClientConnectionManager & connection() const;
		
		/* Return logged in user */
		UserData & user() const;

		/* Return notifications queue */
		std::queue<std::string> & notifications() const;

		/* Shortcut to send {"type": "<type>", "data": <data>} */
		void say(std::string const & type, JSON::Value const & data);

		/* Handle general messages */
		virtual void treatMessage(std::string const & type, JSON::Value const * data);

		/* Pop message from incoming queue and treat it (first with 
		   ClientManager::treatMessage; then with <Subclass>::treatMessage;) */
		void readMessage();

		/* readMessage() until incoming queue empty */
		void readMessages();

		/* Send methods */
		void loadPlayers();

		/* handle message for end of player sale */
		void onEndOfSale(JSON::Dict const & json);

		/* Hooks */
		/* Triggered when we receive a match invitation */
		virtual void onInvite(std::string const & otherUser) {}

		/* Load players in user().players */
		virtual void onPlayersLoad(JSON::List const & players);

		/* Triggered when user's team informations are updated */
		virtual void onTeamInfo(TeamInfo const & team){}
	public:
		/* Create a new client manager with a connection to server, a user 
		   object (might be not initialised), and a queue to put all 
		   notifications for user. A ClientController can empty this queue and
		   show its content to the user. */
		ClientManager(
			net::ClientConnectionManager & connection, 
			UserData & user, 
			std::queue<std::string> & notifications
		);

		/* Copy constructor: init userdata and connection from parent */
		ClientManager(ClientManager const & parent);
};

#endif
