#ifndef __GRAPHIC_CHAMP_MANAGER_HPP
#define __GRAPHIC_CHAMP_MANAGER_HPP

#include <GUINotifier.hpp>
#include <graphics/GraphicManager.hpp>
#include "ChampionshipController.hpp"

class GraphicChampionshipView : public ChampionshipController, public GUI::GraphicManager{
public:
	///constructor
	GraphicChampionshipView(ClientController const & parent, GUI::MainController &controller);
	///destructor
	~GraphicChampionshipView(){}
private:

	void updateChampionships();
	void onChampionshipsLoad();
	void updateCurrentChampionship();
	void onJoinedChampionship();
	void joinChampionship(std::string);
	void onJoinChampionship(bool,std::string const &);
	void leaveChampionship();
	void onLeaveChampionship(bool, std::string const &);
	void seeCurrentChampionship();
	NOTIFIABLE
	bool _wait;
};

#endif