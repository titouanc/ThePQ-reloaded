#ifndef __CLICHAMPMANAGER__HPP
#define __CLICHAMPMANAGER__HPP

#include "ChampionshipManager.hpp"
#include <cli/Menu.hpp>
#include <cli/CLI.hpp>

class CLIChampionshipManager : public ChampionshipManager, public CLI{
public:
	using ChampionshipManager::ChampionshipManager;
	CLIChampionshipManager(ClientManager const & parent);
	void run();
private:
	bool _waitForChamps;
	bool _waitForJoin;

	void joinChampionshipMenu();
	void currentChampionshipMenu();
	void displayChampionships();
	void displayCurrentChampionship();

	virtual void onMatchStart();
	virtual void onJoinChampionship(std::string data);
	virtual void onLeaveChampionship(std::string data);
	virtual void onChampionshipsLoad(JSON::List const & list);
	virtual void onJoinedChampionship(JSON::Value const *);
};

#endif