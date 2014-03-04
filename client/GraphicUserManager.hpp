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
	void submitLoginForm();
	void submitRegisterForm();

	GUI::Button<GraphicUserManager> *_doLoginButton;
	GUI::Button<GraphicUserManager> *_doRegisterButton;

	// Login
	GUI::Button<GraphicUserManager> *_submitLoginButton;
	GUI::Textbox *_usernameTextbox;
	GUI::Textbox *_passwordTextbox;
	// Register
	GUI::Button<GraphicUserManager> *_submitRegisterButton;
	GUI::Textbox *_newUsernameTextbox;
	GUI::Textbox *_newPasswordTextbox;
	GUI::Textbox *_passwordConfirmationTextbox;
};

#endif