#ifndef __CHAMP_MANAGER_HPP
#define __CHAMP_MANAGER_HPP

#include <ClientManager.hpp>
#include <cli/Menu.hpp>
#include <model/Championship.hpp>

class ChampionshipManager : public ClientManager
{
private:
	std::vector<Championship> _champs;
protected:
	std::vector<Championship> const & getChamps() { return _champs; }
	/* attempt to join a championship */
	void joinChampionship(std::string champName);
	/* ask for championships list */
	void loadChampionships();
	/* set joinedChamp to the championship the user joined if any, "" else */
	void updateCurrentChampionship();
	/* leave championship if any */
	void leaveChampionship();
	/* treat Championship specific messages */
	void treatMessage(std::string const & type, JSON::Value const * data);
	/* HOOKS */
	/* received a join championship response */
	virtual void onJoinChampionship(std::string data){}
	/* received a leave championship response */
	virtual void onLeaveChampionship(std::string data){}
	/* received championships list : put it in _champs */
	virtual void onChampionshipsLoad(JSON::List const & json);
public:
	using ClientManager::ClientManager;
	ChampionshipManager(ClientManager const & parent);
};

#endif