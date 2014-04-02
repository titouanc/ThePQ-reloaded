#ifndef __CLICHAMPMANAGER__HPP
#define __CLICHAMPMANAGER__HPP

#include "ChampionshipController.hpp"
#include <cli/Menu.hpp>
#include <cli/CLI.hpp>

class CLIChampionshipView : public ChampionshipController, public CLI{
public:
	using ChampionshipController::ChampionshipController;
	///constructor
	CLIChampionshipView(ClientController const & parent);
	void run();
private:
	bool _waitForChamps;
	bool _waitForJoin;

	void joinChampionshipMenu();
	void currentChampionshipMenu();
	void displayChampionships();
	void displayCurrentChampionship();

	virtual void onMatchStart();
	virtual void onJoinChampionship(bool, std::string const &);
	virtual void onLeaveChampionship(bool, std::string const &);
	virtual void onChampionshipsLoad();
	virtual void onJoinedChampionship();
};

#endif