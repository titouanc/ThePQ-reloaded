#ifndef DEFINE_CLIENTMANAGER_HEADER
#define DEFINE_CLIENTMANAGER_HEADER

#include <network/ClientConnectionManager.hpp>
#include "UserData.hpp"
#include <sstream>
#include <string>

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
			else if (type == net::MSG::MARKET_MESSAGE)
			{
				handleEndOfSaleNotification(data);
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
	private:
		void handleEndOfSaleNotification(JSON::Value const * message){
			std::stringstream res;
			JSON::Dict const & json = DICT(message);
			res << "\n\033[36mMessage : a sale has ended.\033[0m" << endl;
			if(STR(json.get("type")).value()==net::MSG::END_OF_OWNED_SALE_RAPPORT){
				if(STR(json.get(net::MSG::RAPPORT_SALE_STATUS)).value() == net::MSG::PLAYER_NOT_SOLD){
					res << "Your player " << INT(json.get(net::MSG::PLAYER_ID)) << " has not been sold." << endl; 
				}
				else if(STR(json.get(net::MSG::RAPPORT_SALE_STATUS)).value() == net::MSG::PLAYER_SOLD){
					res << "Your player " << INT(json.get(net::MSG::PLAYER_ID)) << " has been sold for " << INT(json.get(net::MSG::BID_VALUE)) 
					<< " to " << STR(json.get(net::MSG::CURRENT_BIDDER)).value() << endl;
				}
			}
			else if(STR(json.get("type")).value()==net::MSG::WON_SALE_RAPPORT){
				std::string owner = STR(json.get(net::MSG::SALE_OWNER)).value();
				res << "You bought player " << INT(json.get(net::MSG::PLAYER_ID)) << " for \33[32m" << INT(json.get(net::MSG::BID_VALUE)) << "\033[0m." <<endl;
				if(owner==net::MSG::GENERATED_BY_MARKET)
					res << "This player did not belong to any team. He was free. Like the wind."<<endl;
				else
					res << "This player comes from " << owner << "'s team." << endl;
			}
			res<<endl;
			_notifications.push(res.str());
		}

	public:
		ClientManager(net::ClientConnectionManager & conn, UserData & user) : 
		_connection(conn), _user(user) {}
		ClientManager(ClientManager const & other) : 
		_connection(other._connection), _user(other._user) {}
};

#endif
