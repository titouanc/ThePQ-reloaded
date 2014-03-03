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
	void submitLoginForm();
	GUI::Button<GraphicUserManager> *_submitButton;
	GUI::Textbox *_usernameTextbox;
	GUI::Textbox *_passwordTextbox;
};

#endif