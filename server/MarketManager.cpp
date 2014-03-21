#include "MarketManager.hpp"


void MarketManager::addPlayerOnMarket(const JSON::Dict &sale, int peer_id){
	Message status(peer_id, _market->addPlayer(sale).clone());
	_outbox.push(status);
}

void MarketManager::sendPlayersOnMarketList(int peer_id){
	Message status(peer_id, _market->allSales().clone());
	_outbox.push(status);
}

void MarketManager::placeBidOnPlayer(const JSON::Dict &bid, int peer_id){
	Message status(peer_id, _market->bid(bid).clone());
	_outbox.push(status);
}