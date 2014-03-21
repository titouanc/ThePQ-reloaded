#ifndef __MARKET_MANAGER_HPP
#define __MARKET_MANAGER_HPP

#include "ServerManager.hpp"
#include "PlayerMarket.hpp"

class MarketManager : public ServerManager
{
protected:
	PlayerMarket _market;

public:
	using ServerManager::ServerManager;
	MarketManager(const ServerManager & parent) : ServerManager(parent)
	{}

    void sendPlayersOnMarketList(int peer_id);
    void addPlayerOnMarket(const JSON::Dict &bid, int peer_id);
    void placeBidOnPlayer(const JSON::Dict &bid, int peer_id);
};

#endif // __MARKET_MANAGER_HPP