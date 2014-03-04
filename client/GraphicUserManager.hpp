#ifndef GRAPHIC_USER_MANAGER_HPP
#define GRAPHIC_USER_MANAGER_HPP 

#include <network/ClientConnectionManager.hpp>
#include "graphics/GraphicManager.hpp"
#include "UserManager.hpp"

class GraphicUserManager : public UserManager, public GUI::GraphicManager 
{
public:
	GraphicUserManager(net::ClientConnectionManager& connection, UserData& user, GUI::MainController &controller);
	~GraphicUserManager(){}
private:
	void displayChoice();	// choice between login and register
	void displayLoginForm();
	void displayRegisterForm();
	void displayTeamNameForm();

	void submitLoginForm();
	void submitRegisterForm();
	void submitTeamNameForm();

	GUI::Button<GraphicUserManager> *_loginChoiceButton;
	GUI::Button<GraphicUserManager> *_registerChoiceButton;

	// Login
	GUI::Button<GraphicUserManager> *_submitLoginButton;
	GUI::Textbox *_usernameTextbox;
	GUI::Textbox *_passwordTextbox;
	// Register
	GUI::Button<GraphicUserManager> *_submitRegisterButton;
	GUI::Textbox *_passwordConfirmationTextbox;

	// Team name
	GUI::Button<GraphicUserManager> *_submitTeamNameButton;
	GUI::Textbox *_teamNameTextbox;
};

#endif