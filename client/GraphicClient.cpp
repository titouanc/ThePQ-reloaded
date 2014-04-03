#include "GraphicClient.hpp"
#include <toolbox.hpp>

/// Constructor
GraphicClient::GraphicClient(NetConfig const &config) :
    _user(), _connection(config.host, config.port), _notifications(),
    _controller(), _gUserController(_connection, _user, _notifications, _controller) {
}

/**
 * Method starting the graphic instance
 */
void GraphicClient::run() {
	bool connected = false;
	while (not connected){
		try {
			_connection.run();
			connected = true;
		}
		catch (...) {
			_gUserController.displayMessage("Could not connect to the server.", {"Retry"});
		}
	}
    _gUserController.run();
    _connection.stop();
}