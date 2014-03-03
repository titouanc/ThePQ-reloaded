#include "GraphicUserManager.hpp"
#include "GraphicStadiumManager.hpp"

using namespace std;
using namespace GUI;

#define USERNAME_TEXTBOX_ID "Username"
#define PASSWORD_TEXTBOX_ID "Password"

GraphicUserManager::GraphicUserManager(net::ClientConnectionManager& connection, UserData& user, GUI::MainController &controller) 
			: UserManager(connection, user), GraphicManager(controller){
	_usernameTextbox = _canvas.addTextbox(USERNAME_TEXTBOX_ID);
	_passwordTextbox = _canvas.addTextbox(PASSWORD_TEXTBOX_ID);
	_submitButton = _canvas.addButton<GraphicUserManager>(&GraphicUserManager::submitLoginForm, this, "Login");

	_usernameTextbox->setPosition(900, 300);
	_passwordTextbox->setPosition(900, 360);
	_submitButton->setPosition(900, 420);
	displayCanvas();
	run();
}

void GraphicUserManager::submitLoginForm(){
	try{
		doLoginUser(_usernameTextbox->getText(), _passwordTextbox->getText());
		GraphicStadiumManager gsm(_connection, _user, _controller);
	}
	catch (UserNotFoundException & e)
	{
		cout << "\nUser not found" << endl;
	}
	catch (WrongPasswordException & e)
	{
		cout << "\nWrong password" << endl;
	}
	catch (AlreadyLoggedInException & e)
	{
		cout << "\nYou're already logged in from another location" << endl;
	}
}

