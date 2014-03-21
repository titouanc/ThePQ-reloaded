#include "StadiumManager.hpp"
#include <json/json.hpp>
#include <vector>
#include <model/Installation.hpp>

void StadiumManager::sendInstallationsList(int peer_id)
{
	JSON::List jsonInst;
	std::vector<Installation*> & insts = _users[peer_id]->getTeam().getInstallations();
	for(size_t i = 0;i<insts.size();++i){
		jsonInst.append(JSON::Dict(*insts[i]));
	}
	JSON::Dict msg;
	msg.set("type", net::MSG::INSTALLATIONS_LIST);
	msg.set("data", jsonInst);
	_outbox.push(Message(peer_id, msg.clone()));
}

void StadiumManager::upgradeInstallation(int peer_id, size_t i)
{
	bool res = _users[peer_id]->getTeam().upgradeInstallation(i);
	JSON::Dict msg;
	msg.set("type", net::MSG::INSTALLATION_UPGRADE);
	msg.set("data", JSON::Bool(res));
	_outbox.push(Message(peer_id, msg.clone()));
	JSON::Dict cache;
	cache.set("funds",_users[peer_id]->getTeam().getFunds());
	sendTeamInfos(cache,peer_id);
}

void StadiumManager::downgradeInstallation(int peer_id, size_t i)
{
	bool res = _users[peer_id]->getTeam().downgradeInstallation(i);
	JSON::Dict msg;
	msg.set("type", net::MSG::INSTALLATION_DOWNGRADE);
	msg.set("data", JSON::Bool(res));
	_outbox.push(Message(peer_id, msg.clone()));
	JSON::Dict cache;
	cache.set("funds",_users[peer_id]->getTeam().getFunds());
	sendTeamInfos(cache,peer_id);
}