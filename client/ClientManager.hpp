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
		std::queue<JSON::Dict> & _notifications;

	protected:
		/* Return connection object */
		net::ClientConnectionManager & connection() const;
		
		/* Return logged in user */
		UserData & user() const;

		/* Return nb notifications */
		int getNbNotifications() const;

		/* Return notifications queue */
		std::queue<JSON::Dict> & notifications() const;

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


		/* Notifications handlers */
		/* pop() a notification and handles it */
		void handleNotification();

		/* handle message for end of player sale */
		std::string onEndOfSale(JSON::Dict const & json);

		/* Reponse to a pending match notification */
		void readyForMatch();
		void withdrawFromMatch();

		/* response to invitation */
		void acceptInvitationFromUser(std::string);
		void denyInvitationFromUser(std::string);

		/* Hooks */
		/* Displays a message */
		virtual void onMessage(std::string const & message) {}

		/* Triggered when we receive a match invitation */
		virtual void onInvite(std::string const & otherUser) {}

		/* Triggered to start a match */
		virtual void onMatchStart(){}

		/* Triggered when a championship match is ready to be played; answer has to be ready or withdraw */
		virtual void onMatchPending(){}

		/* Called when : handled notification -> responded to it -> response from server */
		virtual void onNotificationResponse(bool,std::string const &,std::string const &){}

		/* Load players in user().players */
		virtual void onPlayersLoad(JSON::List const & players);

		/* Triggered when user's team informations are updated */
		virtual void onTeamInfo(UserData const & user);

	public:
		/* Create a new client manager with a connection to server, a user 
		   object (might be not initialised), and a queue to put all 
		   notifications for user. A ClientController can empty this queue and
		   show its content to the user. */
		ClientManager(
			net::ClientConnectionManager & connection, 
			UserData & user, 
			std::queue<JSON::Dict> & notifications
		);

		/* Copy constructor: init userdata and connection from parent */
		ClientManager(ClientManager const & parent);
};

#endif
