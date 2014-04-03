#ifndef GRAPHIC_USER_MANAGER_HPP
#define GRAPHIC_USER_MANAGER_HPP 

#include <network/ClientConnectionManager.hpp>
#include <graphics/GraphicManager.hpp>
#include "UserController.hpp"
#include <GUINotifier.hpp>

class GraphicUserView : public UserController, public GUI::GraphicManager 
{
public:
	GraphicUserView(net::ClientConnectionManager& connection, UserData& user, std::queue<JSON::Dict> & notifications, GUI::MainController &controller);
	~GraphicUserView(){}
private:
	/* Display methods */
	void displayChoice();	// choice between login and register
	void displayLoginForm();
	void displayRegisterForm();
	void displayTeamNameForm();
	void displayMainMenu();

	/* Buttons methods */
	void displayCredits();
	void submitLoginForm();
	void submitRegisterForm();
	void submitTeamNameForm();
	void goToFriendlyGame();
	void goToStadium();
	void goToMarket();
	void goToPlayers();
	void goToChampionships();

	/* Hooks */
	void onAskTeamName();

	void onLoginOK();
	void onLoginError(std::string const & error);

	void onTeamNameOK();
	void onTeamNameError(std::string const & error);

	void onRegisterUserOK();
	void onRegisterUserError(std::string const & data);

	NOTIFIABLE

};

#endif