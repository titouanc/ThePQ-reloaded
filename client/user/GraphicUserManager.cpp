#include "GraphicUserManager.hpp"
#include <stadium/GraphicStadiumManager.hpp>

using namespace std;
using namespace GUI;

#define USERNAME_TEXTBOX_ID "Username"
#define PASSWORD_TEXTBOX_ID "Password"
#define NEW_USERNAME_TEXTBOX_ID "New Username"
#define NEW_PASSWORD_TEXTBOX_ID "New Password"
#define NEW_PASSWORD_CONFIRMATION_TEXTBOX_ID "Confirm Password"
#define TEAM_NAME_TEXTBOX_ID "Team Name"

GraphicUserManager::GraphicUserManager(net::ClientConnectionManager& connection, UserData& user, std::queue<std::string> & notifications, GUI::MainController &controller) : 
	UserManager(connection, user, notifications), GraphicManager(controller)
{
	
	_loginChoiceButton = _canvas.addButton<GraphicUserManager>(&GraphicUserManager::displayLoginForm, this, "Login");
	_registerChoiceButton = _canvas.addButton<GraphicUserManager>(&GraphicUserManager::displayRegisterForm, this, "Register");
	_loginChoiceButton->setPosition(900, 350);
	_registerChoiceButton->setPosition(900, 410);

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
}

void GraphicUserManager::displayChoice()
{
	_usernameTextbox->hide();
	_passwordTextbox->hide();
	_passwordConfirmationTextbox->hide();
	_submitRegisterButton->hide();
	_submitLoginButton->hide();

	_loginChoiceButton->unhide();
	_registerChoiceButton->unhide();
	redrawCanvas();
}

void GraphicUserManager::displayLoginForm()
{
	_passwordConfirmationTextbox->hide();
	_submitRegisterButton->hide();
	_loginChoiceButton->hide();
	_registerChoiceButton->hide();

	_usernameTextbox->unhide();
	_passwordTextbox->unhide();
	_submitLoginButton->unhide();
	redrawCanvas();
}

void GraphicUserManager::displayRegisterForm()
{
	_loginChoiceButton->hide();
	_registerChoiceButton->hide();
	_submitLoginButton->hide();

	_usernameTextbox->unhide();
	_passwordTextbox->unhide();
	_passwordConfirmationTextbox->unhide();
	_submitRegisterButton->unhide();
	redrawCanvas();
}

void GraphicUserManager::displayTeamNameForm()
{
	_submitTeamNameButton = _canvas.addButton<GraphicUserManager>(&GraphicUserManager::submitTeamNameForm, this, "Let's gooooo!");
	_teamNameTextbox = _canvas.addTextbox(TEAM_NAME_TEXTBOX_ID);
	
	int center = _controller.window.getSize().x/2;
	_teamNameTextbox->setPosition(center-_teamNameTextbox->getWidth()/2, 300);
	_submitTeamNameButton->setPosition(center-_submitTeamNameButton->getWidth()/2, 360);

	_usernameTextbox->hide();
	_passwordTextbox->hide();
	_submitLoginButton->hide();
	redrawCanvas();
}

void GraphicUserManager::submitLoginForm()
{
	_wait = true;
	loginUser(_usernameTextbox->getText(), _passwordTextbox->getText());
	while (_wait){
		readEvent();
		readMessages();
	}
}

void GraphicUserManager::submitRegisterForm()
{
	if (_passwordTextbox->getText() != _passwordConfirmationTextbox->getText()){
		onRegisterUserError("The 2 passwords doesn't match");
	} else {
		_wait = true;
		registerUser(_usernameTextbox->getText(), _passwordTextbox->getText());
		while (_wait){
			readEvent();
			readMessages();
		}
	}
}

void GraphicUserManager::submitTeamNameForm()
{
	_wait = true;
	chooseTeamName(user().username, _teamNameTextbox->getText());
	while (_wait){
		readEvent();
		readMessages();
	}
}


/* HOOKS */
void GraphicUserManager::onLoginOK()
{
	GraphicStadiumManager stadium(*this, _controller);
	stadium.run();
	_wait = false;
}

void GraphicUserManager::onLoginError(std::string const & err)
{
	std::cout << err << std::endl;
	_wait = false;
}

void GraphicUserManager::onTeamNameOK()
{
	onLoginOK();
}

void GraphicUserManager::onTeamNameError(std::string const & err)
{
	std::cout << err << std::endl;
	_wait = false;
}

void GraphicUserManager::onRegisterUserOK()
{
	displayLoginForm();
	_wait = false;
}

void GraphicUserManager::onRegisterUserError(std::string const & err)
{
	std::cout << err << std::endl;
	_wait = false;
}

void GraphicUserManager::onAskTeamName()
{
	displayTeamNameForm();
}
