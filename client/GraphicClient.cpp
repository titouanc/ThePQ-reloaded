#include "GraphicClient.hpp"
#include <toolbox.hpp>

/// Constructor
GraphicClient::GraphicClient(NetConfig const &config) :
    _user(), _connection(config.host, config.port), _notifications(),
    _controller(), _gUserManager(_connection, _user, _notifications, _controller) {
}

/**
 *Method starting the graphic instance
 */
void GraphicClient::run() {
	bool connected = false;
	while (not connected){
		try {
			_connection.run();
			connected = true;
		}
		catch (...) {
			_gUserManager.displayMessage("Could not connect to the server.", {"Retry"});
		}
	}
    _gUserManager.run();
    _connection.stop();
}