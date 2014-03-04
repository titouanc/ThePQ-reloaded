#include "GraphicUserManager.hpp"
#include "GraphicStadiumManager.hpp"

using namespace std;
using namespace GUI;

#define USERNAME_TEXTBOX_ID "Username"
#define PASSWORD_TEXTBOX_ID "Password"
#define NEW_USERNAME_TEXTBOX_ID "New Username"
#define NEW_PASSWORD_TEXTBOX_ID "New Password"
#define NEW_PASSWORD_CONFIRMATION_TEXTBOX_ID "Confirm Password"

GraphicUserManager::GraphicUserManager(net::ClientConnectionManager& connection, UserData& user, GUI::MainController &controller) 
			: UserManager(connection, user), GraphicManager(controller){
	
	_doLoginButton = _canvas.addButton<GraphicUserManager>(&GraphicUserManager::displayLoginForm, this, "Login");
	_doRegisterButton = _canvas.addButton<GraphicUserManager>(&GraphicUserManager::displayRegisterForm, this, "Register");
	_doLoginButton->setPosition(900, 350);
	_doRegisterButton->setPosition(900, 410);

	// Login form
	_usernameTextbox = _canvas.addTextbox(USERNAME_TEXTBOX_ID);
	_passwordTextbox = _canvas.addTextbox(PASSWORD_TEXTBOX_ID);
	_submitLoginButton = _canvas.addButton<GraphicUserManager>(&GraphicUserManager::submitLoginForm, this, "Login");
	_usernameTextbox->setPosition(900, 300);
	_passwordTextbox->setPosition(900, 360);
	_submitLoginButton->setPosition(900, 420);

	// Registering form
	_passwordConfirmationTextbox = _canvas.addTextbox(NEW_PASSWORD_CONFIRMATION_TEXTBOX_ID);
	_submitRegisterButton = _canvas.addButton<GraphicUserManager>(&GraphicUserManager::submitRegisterForm, this, "Register");
	_passwordConfirmationTextbox->setPosition(900, 420);
	_submitRegisterButton->setPosition(900, 480);

	displayChoice(); // only displays two buttons : login and register.
	// each button will display the corresponding form.

	displayCanvas();
	run();
}

void GraphicUserManager::displayChoice(){
	_usernameTextbox->hide();
	_passwordTextbox->hide();
	_passwordConfirmationTextbox->hide();
	_submitRegisterButton->hide();
	_submitLoginButton->hide();

	_doLoginButton->unhide();
	_doRegisterButton->unhide();
	redrawCanvas();
}

void GraphicUserManager::displayLoginForm(){
	_passwordConfirmationTextbox->hide();
	_submitRegisterButton->hide();
	_doLoginButton->hide();
	_doRegisterButton->hide();

	_usernameTextbox->unhide();
	_passwordTextbox->unhide();
	_submitLoginButton->unhide();
	redrawCanvas();
}

void GraphicUserManager::displayRegisterForm(){
	_doLoginButton->hide();
	_doRegisterButton->hide();
	_submitLoginButton->hide();

	_usernameTextbox->unhide();
	_passwordTextbox->unhide();
	_passwordConfirmationTextbox->unhide();
	_submitRegisterButton->unhide();
	redrawCanvas();
}

void GraphicUserManager::submitLoginForm(){
	try{
		loginUser(_usernameTextbox->getText(), _passwordTextbox->getText());
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

void GraphicUserManager::submitRegisterForm(){
	try{
		if (_passwordTextbox->getText() != _passwordConfirmationTextbox->getText()){
			cout << "passwords not identical" << endl;
		}
		else {
			registerUser(_usernameTextbox->getText(), _passwordTextbox->getText());
			displayLoginForm();
		}
	}
	catch (UserAlreadyExistsException & e)
	{
		cout << "\nUser already exists!" << endl;
	}
}

