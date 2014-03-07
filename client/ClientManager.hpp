#ifndef DEFINE_CLIENTMANAGER_HEADER
#define DEFINE_CLIENTMANAGER_HEADER

#include <network/ClientConnectionManager.hpp>
#include "UserData.hpp"
#include <sstream>
#include <string>
#include <queue>

class ClientManager {
	private:
		net::ClientConnectionManager & _connection;
		UserData & _user;
		std::queue<std::string> & _notifications;

		/* handle message for end of player sale */
		void onEndOfSale(JSON::Dict const & json);
		void onPlayersLoad(JSON::List const & players);

	protected:
		/* Return connection object */
		net::ClientConnectionManager & connection() const;
		
		/* Return logged in user */
		UserData & user() const;

		/* Return notifications queue */
		std::queue<std::string> & notifications() const;

		/* Shortcut to send {"type": "<type>", "data": <data>} */
		virtual void say(std::string const & type, JSON::Value const & data);

		/* Handle general messages */
		virtual void treatMessage(std::string const & type, JSON::Value const * data);

		/* Pop and treat all messages in incoming queue */
		void readMessages();

		/* Send methods */
		void loadPlayers();

		/* Hook on invitation */
		virtual void onInvite(std::string) {}

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
