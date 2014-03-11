#ifndef GRAPHIC_USER_MANAGER_HPP
#define GRAPHIC_USER_MANAGER_HPP 

#include <network/ClientConnectionManager.hpp>
#include <graphics/GraphicManager.hpp>
#include "UserManager.hpp"

class GraphicUserManager : public UserManager, public GUI::GraphicManager 
{
public:
	GraphicUserManager(net::ClientConnectionManager& connection, UserData& user, std::queue<JSON::Dict> & notifications, GUI::MainController &controller);
	~GraphicUserManager(){}
private:
	/* Display methods */
	void displayChoice();	// choice between login and register
	void displayLoginForm();
	void displayRegisterForm();
	void displayTeamNameForm();
	void displayMainMenu();

	/* Buttons methods */
	void submitLoginForm();
	void submitRegisterForm();
	void submitTeamNameForm();
	void goToFriendlyGame();
	void goToStadium();
	void goToMarket();
	void goToPlayers();

	/* Hooks */
	void onAskTeamName();

	void onLoginOK();
	void onLoginError(std::string const & error);

	void onTeamNameOK();
	void onTeamNameError(std::string const & error);

	void onRegisterUserOK();
	void onRegisterUserError(std::string const & data);

	bool _wait;
};

#endif