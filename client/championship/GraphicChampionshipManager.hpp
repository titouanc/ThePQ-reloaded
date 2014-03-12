#ifndef __GRAPHIC_CHAMP_MANAGER_HPP
#define __GRAPHIC_CHAMP_MANAGER_HPP

#include <GUINotifier.hpp>
#include <graphics/GraphicManager.hpp>
#include "ChampionshipManager.hpp"

class GraphicChampionshipManager : public ChampionshipManager, public GUI::GraphicManager{
public:
	GraphicChampionshipManager(ClientManager const & parent, GUI::MainController &controller);
	~GraphicChampionshipManager(){}
private:

	void updateChampionships();
	void onChampionshipsLoad();
	void updateCurrentChampionship();
	void onJoinedChampionship();
	void joinChampionship(std::string);
	NOTIFIABLE
	bool _wait;
};

#endif