#ifndef DEFINE_CLIENTMANAGER_HEADER
#define DEFINE_CLIENTMANAGER_HEADER

#include <network/ClientConnectionManager.hpp>
#include "UserData.hpp"

class ClientManager {
	private:
		net::ClientConnectionManager & _connection;
		UserData & _user;
		std::queue<std::string> _notifications;

	protected:
		net::ClientConnectionManager & connection() const {return _connection;}
		UserData & user() const {return _user;}
		void say(std::string const & type, JSON::Value const & data){
			JSON::Dict msg = {
				{"type", JSON::String(type)},
				{"data", data}
			};
			_connection.send(msg);
		}

		virtual void treatMessage(std::string type, JSON::Value const * data)
		{
			if (type == net::MSG::TEAM_INFOS)
			{

			}
			else if (type == net::MSG::BID_ENDED)
			{
				_notifications.push("Bid ended!");
			}
		}

		void readMessages() {
	        while (_connection.hasMessage()){
	        	JSON::Value * msg = _connection.popMessage();
	            JSON::Dict const & dict = DICT(msg);
				std::string messageType = STR(dict.get("type"));
	            ClientManager::treatMessage(messageType, dict.get("data"));
	            treatMessage(messageType, dict.get("data")); /* virtuelle */
	            delete msg;
	        }
		}

	public:
		ClientManager(net::ClientConnectionManager & conn, UserData & user) : 
		_connection(conn), _user(user) {}
		ClientManager(ClientManager const & other) : 
		_connection(other._connection), _user(other._user) {}
};

#endif
