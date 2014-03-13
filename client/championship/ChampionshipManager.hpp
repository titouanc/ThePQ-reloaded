#ifndef __CHAMP_MANAGER_HPP
#define __CHAMP_MANAGER_HPP

#include <ClientManager.hpp>
#include <cli/Menu.hpp>
#include <model/Championship.hpp>

class ChampionshipManager : public ClientManager
{
protected:
	std::vector<Championship> _champs;
	std::vector<Championship> const & getChamps() { return _champs; }
	/* attempt to join a championship */
	void joinChampionship(std::string champName);
	/* ask for championships list */
	void loadChampionships();
	/* ask for current joined championship */
	void joinedChampionship();
	/* leave championship if any */
	void leaveCurrentChampionship();
	/* treat Championship specific messages */
	void treatMessage(std::string const & type, JSON::Value const * data);
	/* HOOKS */
	/* received a join championship response */
	virtual void onJoinChampionship(bool,std::string){}
	/* received a leave championship response */
	virtual void onLeaveChampionship(bool,std::string){}
	/* received championships list : put it in _champs */
	virtual void onChampionshipsLoad(){}
	/* received current joined champ, put in joinedChamp if !NULL */
	virtual void onJoinedChampionship(){}
public:
	using ClientManager::ClientManager;
	ChampionshipManager(ClientManager const & parent);
};

#endif