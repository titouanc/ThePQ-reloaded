#ifndef DEFINE_CLIENTMANAGER_HEADER
#define DEFINE_CLIENTMANAGER_HEADER

#include <network/ClientConnectionManager.hpp>
#include "UserData.hpp"

class ClientManager {
	private:
		net::ClientConnectionManager & _connection;
		UserData & _user;

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

	public:
		ClientManager(net::ClientConnectionManager & conn, UserData & user) : 
		_connection(conn), _user(user) {}
		ClientManager(ClientManager const & other) : 
		_connection(other._connection), _user(other._user) {}
};

#endif
