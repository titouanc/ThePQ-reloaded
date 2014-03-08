#include "GraphicClient.hpp"

GraphicClient::GraphicClient(NetConfig const &config) :
    _user(), _connection(config.host, config.port), _notifications(),
    _controller(), _gUserManager(_connection, _user, _notifications, _controller) {
}

void GraphicClient::run() {
        _connection.run();
        _gUserManager.run();
}