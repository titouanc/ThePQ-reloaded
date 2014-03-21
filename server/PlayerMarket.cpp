#include "PlayerMarket.hpp"
#include <Constants.hpp>
#include <sys/stat.h>
#include <stdio.h>
#include <Exception.hpp>
#include <model/MemoryAccess.hpp>
#include <math.h>
#include <model/Gear.hpp>
#include <model/RandomNameGenerator.hpp>
#include <utility>


PlayerMarket::PlayerMarket(const ServerManager & parent): ServerManager(parent), _sales(),
_manager(), _generator(), _runManager(true), _runGenerator(true), _locker(PTHREAD_MUTEX_INITIALIZER) {
	MemoryAccess::load(_sales);
	startManager();
	startGenerator();
}

PlayerMarket::~PlayerMarket(){
	_runManager = false;
	_runGenerator = false;
	for(size_t i=0;i<_sales.size();++i){
		delete _sales[i];
	}
}

void * saleManager(void * p){
	PlayerMarket *market = static_cast<PlayerMarket*>(p);
	while(market->_runManager){
		sleep(1); //One iteration = --timeLeft for each sale
		if(pthread_mutex_lock(&(market->_locker)) != 0)
			throw "Couldn't acquire lock for PlayerMarket";
		for(size_t i = 0; i<market->_sales.size();++i){
			Sale & sale = *(market->_sales[i]);
			sale.decTime();
			if(sale.getTimeLeft()==0){
				sale.resolveEndOfTurn();
				if(sale.isOver()){
					market->resolveEndOfSale(&sale);
					MemoryAccess::removeObject(sale);
					delete market->_sales[i];
					market->_sales.erase(market->_sales.begin()+i);
				}
			}
		}
		pthread_mutex_unlock(&(market->_locker));
	}
	return NULL;
}

void * saleGenerator(void * p){
	RandomNameGenerator gen;
	PlayerMarket *market = static_cast<PlayerMarket*>(p);
	while(market->_runGenerator){
		sleep(20);	//TODO : log, nbrUsersConnected(), etc.
		Player generated;
		generated.setName(gen.getRandomName());
		generated.setMemberID();
		market->createSale(generated.getMemberID(), generated.estimatedValue(), generated, net::MSG::GENERATED_BY_MARKET);
	}
	return NULL;
}

void PlayerMarket::startManager(){
	if(pthread_create(&_manager, NULL, saleManager, this) != 0){
		throw "Error occured when creating manager thread";
	}
}

void PlayerMarket::startGenerator(){
	if(pthread_create(&_generator, NULL, saleGenerator, this) != 0){
		throw "Error occured when creating generator thread";
	}
}

void PlayerMarket::createSale(int player_id, int value, Player& player, std::string username){
	if(pthread_mutex_lock(&_locker) != 0)
		throw "Couldn't acquire lock for PlayerMarket";
	_sales.push_back(new Sale(value, username, player_id, player));
	Sale *added = getSale(player_id);
	added->save();
	pthread_mutex_unlock(&_locker);
}

void PlayerMarket::transfert(std::string fromName, std::string toName, int id, int bidValue, Sale* sale){
	User* from = getUserByName(fromName);
	Player toTransfert;
	if(from == NULL){	
		if(fromName != net::MSG::GENERATED_BY_MARKET){ //User not connected : load.
			Team fromTeam(fromName);
			fromTeam.load();
			toTransfert = fromTeam.getPlayer(id);
			fromTeam.removePlayer(id);
			fromTeam.getPayed(bidValue);
			fromTeam.save();
		}
		else{	//Sale generated by market.
			toTransfert = sale->getPlayer();
		}
	}
	else{				//User connected : use ref, no need to load.
		Team & fromTeam = from->getTeam();
		toTransfert = fromTeam.getPlayer(id);
		fromTeam.removePlayer(id);
		fromTeam.getPayed(bidValue);
		fromTeam.save();
	}
	toTransfert.setOwner(toName);
	User* to= getUserByName(toName);
	//N.B. : no need to call buy(), since the team alrdy paid the amount in handleNewBid().
	if(to == NULL){
		Team toTeam(toName);
		toTeam.load();
		toTeam.addPlayer(toTransfert);
		toTeam.save();
	}
	else{
		Team & toTeam = to->getTeam();
		toTeam.addPlayer(toTransfert);
		toTeam.save();
	}

}
void PlayerMarket::resolveEndOfSale(Sale * sale){
	if(sale->getCurrentBidder().empty() and sale->getOwner() != net::MSG::GENERATED_BY_MARKET){//Player not sold
		JSON::Dict toOwner;
		toOwner.set("type",net::MSG::END_OF_OWNED_SALE_RAPPORT);
		toOwner.set(net::MSG::RAPPORT_SALE_STATUS, net::MSG::PLAYER_NOT_SOLD);
		toOwner.set(net::MSG::PLAYER_ID, sale->getID());
		sendMessageToUser(sale->getOwner(), toOwner);
	}
	else if (!(sale->getCurrentBidder().empty())){
		transfert(sale->getOwner(),sale->getCurrentBidder(),sale->getID(),sale->getBidValue(), sale);
		JSON::Dict toOwner, toWinner;
		if(sale->getOwner() != net::MSG::GENERATED_BY_MARKET){
			toOwner.set("type",net::MSG::END_OF_OWNED_SALE_RAPPORT);
			toOwner.set(net::MSG::RAPPORT_SALE_STATUS, net::MSG::PLAYER_SOLD);
			toOwner.set(net::MSG::PLAYER_ID, sale->getID());
			toOwner.set(net::MSG::BID_VALUE, sale->getBidValue());
			toOwner.set(net::MSG::CURRENT_BIDDER, sale->getCurrentBidder());
			sendMessageToUser(sale->getOwner(), toOwner);
		}

		toWinner.set("type",net::MSG::WON_SALE_RAPPORT);
		toWinner.set(net::MSG::PLAYER_ID,sale->getID());
		toWinner.set(net::MSG::BID_VALUE, sale->getBidValue());
		toWinner.set(net::MSG::SALE_OWNER, sale->getOwner());
		sendMessageToUser(sale->getCurrentBidder(), toWinner);
	}
}

Sale * PlayerMarket::getSale(int id){
	for(size_t i = 0; i<_sales.size();++i){
		if(_sales[i]->getID() == id){return _sales[i];}
	}
	return NULL;
}

JSON::Dict PlayerMarket::allSales() {
	JSON::Dict response;
	response.set("type", net::MSG::PLAYERS_ON_MARKET_LIST);
	JSON::List sales;
	if(pthread_mutex_lock(&_locker) != 0)
		throw "Couldn't acquire lock for PlayerMarket";
	for(size_t i=0;i<_sales.size();++i){
		sales.append(JSON::Dict(*(_sales[i])));
	}
	response.set("data", sales);
	pthread_mutex_unlock(&_locker);
	return response;
}

JSON::Dict PlayerMarket::addPlayer(const JSON::Dict &json){
	JSON::Dict response = JSON::Dict();
	response.set("type", net::MSG::ADD_PLAYER_ON_MARKET_QUERY);
	int player_id = INT(json.get(net::MSG::PLAYER_ID));
	int bidValue = INT(json.get(net::MSG::BID_VALUE));
	std::string username = STR(json.get(net::MSG::USERNAME));
	if(getSale(player_id) != NULL){
		response.set("data",net::MSG::PLAYER_ALREADY_ON_MARKET);
	}
	else if((getUserByName(username)->getTeam().getPlayers().size() + ownedSales(username))<= gameconfig::MIN_PLAYERS){
		response.set("data",net::MSG::NOT_ENOUGH_PLAYERS);
	}
	else{
		Player player(player_id, username);
		MemoryAccess::load(player);
		createSale(player_id, bidValue, player, username);
		response.set("data", net::MSG::PLAYER_ADDED_ON_MARKET);
	}
	return response;
}

JSON::Dict PlayerMarket::bidOn(const JSON::Dict &json){
	JSON::Dict response = JSON::Dict();
	response.set("type", net::MSG::BID_ON_PLAYER_QUERY);
	std::string username = STR(json.get(net::MSG::USERNAME)).value();
	int player_id = INT(json.get(net::MSG::PLAYER_ID));
	int bid_value = INT(json.get(net::MSG::BID_VALUE));
 	if((getUserByName(username)->getTeam().getPlayers().size() + winningSales(username)) >= gameconfig::MAX_PLAYERS)
		response.set("data", net::MSG::TOO_MANY_PLAYERS);
	else if(getUserByName(username)->getTeam().getFunds() < bid_value){
		response.set("data",net::MSG::INSUFFICIENT_FUNDS);
	}
	else{
		try{
			if(pthread_mutex_lock(&_locker) != 0)
				throw "Couldn't acquire lock for PlayerMarket";
			Sale * sale = getSale(player_id);
			if(sale == NULL or sale->isOver())
				throw bidEndedException();
			std::pair<std::string, int> previous = sale->placeBid(username, bid_value);
			handleNewBid(previous.first,username,previous.second,bid_value);
			response.set("data", net::MSG::BID_PLACED);
		}
		catch(bidEndedException e){
			response.set("data", net::MSG::BID_ENDED);
		}
		catch(bidValueNotUpdatedException e){
			response.set("data", net::MSG::BID_VALUE_NOT_UPDATED);
		}
		catch(turnException e){
			response.set("data", net::MSG::BID_TURN_ERROR);
		}
		catch(bidOnYourPlayerException e){
			response.set("data", net::MSG::CANNOT_BID_ON_YOUR_PLAYER);
		}
		catch(lastBidderException e){
			response.set("data", net::MSG::LAST_BIDDER);
		}
		pthread_mutex_unlock(&_locker);
	}
	return response;
}

void PlayerMarket::handleNewBid(std::string previousBidder, std::string currentBidder, int previousAmount, int currentAmount){
	Team & currentTeam = getUserByName(currentBidder)->getTeam();
	currentTeam.buy(currentAmount);
	currentTeam.saveInfos();
	JSON::Dict toSend;
	toSend.set("funds",currentTeam.getFunds());
	sendTeamInfos(toSend, getPeerID(currentBidder));

	if(!previousBidder.empty()){
		User* previousUser = getUserByName(previousBidder);
		if (previousUser == NULL){ 			//User not connected
			Team previousTeam(previousBidder);
			previousTeam.loadInfos(); 		//Do not load players nor installations, since it's not needed
			previousTeam.getPayed(previousAmount);
			previousTeam.saveInfos();
		}
		else{								//User connected, dynamically saves the change too
			Team & previousTeam = previousUser->getTeam();
			previousTeam.getPayed(previousAmount);
			previousTeam.saveInfos();

			JSON::Dict toSend;
			toSend.set("funds",previousTeam.getFunds());
			sendTeamInfos(toSend, getPeerID(previousBidder));
		}
	}
}

void PlayerMarket::sendMessageToUser(std::string username, const JSON::Dict & message){
	sendMarketMessage(username, message);
}

int PlayerMarket::ownedSales(std::string username){
	int res=0;
	for(size_t i=0;i<_sales.size();++i){
		if(_sales[i]->getOwner() == username){
			++res;
		}
	}
	return res;
}

int PlayerMarket::winningSales(std::string username){
	int res=0;

	for(size_t i=0;i<_sales.size();++i){
		if(_sales[i]->getCurrentBidder() == username){
			++res;
		}
	}
	return res;
}
void PlayerMarket::addPlayerOnMarket(const JSON::Dict &sale, int peer_id){
	Message status(peer_id, addPlayer(sale).clone());
	_outbox.push(status);
}

void PlayerMarket::sendPlayersOnMarketList(int peer_id){
	Message status(peer_id, allSales().clone());
	_outbox.push(status);
}

void PlayerMarket::placeBidOnPlayer(const JSON::Dict &bid, int peer_id){
	Message status(peer_id, bidOn(bid).clone());
	_outbox.push(status);
}

void PlayerMarket::sendMarketMessage(std::string const &username, const JSON::Dict &message){
	JSON::Dict toSend;
	toSend.set("type",net::MSG::MARKET_MESSAGE);
	toSend.set("data",message);

	int to_peer = getPeerID(username);
	if (to_peer >= 0){
		/* User currently connected to server */
		Message status(to_peer, toSend.clone());
		_outbox.push(status); 
	} else {
		User *user = User::load(username);
		if (user != NULL){
			user->sendOfflineMsg(toSend);
			delete user;
		}
	}
}