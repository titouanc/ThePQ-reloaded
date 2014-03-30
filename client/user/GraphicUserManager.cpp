#include "GraphicUserManager.hpp"
#include <stadium/GraphicStadiumManager.hpp>
#include <market/GraphicMarketManager.hpp>
#include <fgame/GUIFGameManager.hpp>
#include <team/GraphicTeamManager.hpp>
#include <championship/GraphicChampionshipManager.hpp>

using namespace std;
using namespace GUI;

///who doesn't love a good define here and there :)
#define USERNAME_TEXTBOX_ID "Username"
#define PASSWORD_TEXTBOX_ID "Password"
#define NEW_USERNAME_TEXTBOX_ID "New Username"
#define NEW_PASSWORD_TEXTBOX_ID "New Password"
#define NEW_PASSWORD_CONFIRMATION_TEXTBOX_ID "Confirm Password"
#define TEAM_NAME_TEXTBOX_ID "Team Name"

///Conctructor
GraphicUserManager::GraphicUserManager(
	    net::ClientConnectionManager& connection, 
    	UserData& user, 
    	std::queue<JSON::Dict> & notifications, 
    	GUI::MainController &controller
	) : 
    UserManager(connection, user, notifications), 
    GraphicManager(controller)
{
	_canvas.setBackgroundImage("SplashScreenBG.png");
	displayCanvas();
	displayChoice();
}

 /**
  * Method handling the registration interface
  */
void GraphicUserManager::displayChoice()
{
	clear();

	Button<GraphicUserManager> & loginButton = _canvas.addButton<GraphicUserManager>(
		&GraphicUserManager::displayLoginForm, this, "Login");
	loginButton.setPosition(900, 350);
	loginButton.setBackgroundColor(DARK_BUTTON_BACKGROUND_COLOR);

	Button<GraphicUserManager> & registerButton = _canvas.addButton<GraphicUserManager>(
		&GraphicUserManager::displayRegisterForm, this, "Register");
	registerButton.setPosition(900, 410);
	registerButton.setBackgroundColor(DARK_BUTTON_BACKGROUND_COLOR);
	
	redrawCanvas();
}

/**
  * Method handling the login form interface
  */
void GraphicUserManager::displayLoginForm()
{
	clear();
	
	_canvas.addTextbox(USERNAME_TEXTBOX_ID).setPosition(900, 300);
	_canvas.addTextbox(PASSWORD_TEXTBOX_ID).setPosition(900, 360);
	Button<GraphicUserManager> & loginButton = _canvas.addButton<GraphicUserManager>(
		&GraphicUserManager::submitLoginForm, this, "Login");
	loginButton.setPosition(900, 420);
	loginButton.setBackgroundColor(DARK_BUTTON_BACKGROUND_COLOR);
	
	redrawCanvas();
}

/**
  * Method handling the registration interface
  */
void GraphicUserManager::displayRegisterForm()
{
	clear();
	
	_canvas.addTextbox(USERNAME_TEXTBOX_ID).setPosition(900, 300);
	_canvas.addTextbox(PASSWORD_TEXTBOX_ID).setPosition(900, 360);
	_canvas.addTextbox(NEW_PASSWORD_CONFIRMATION_TEXTBOX_ID).setPosition(900, 420);
	Button<GraphicUserManager> & registerButton = _canvas.addButton<GraphicUserManager>(
		&GraphicUserManager::submitRegisterForm, this, "Register");
	registerButton.setPosition(900, 480);
	registerButton.setBackgroundColor(DARK_BUTTON_BACKGROUND_COLOR);
	
	redrawCanvas();
}

/**
  * Method handling the team configuration interface
  */
void GraphicUserManager::displayTeamNameForm()
{
	clear();

	_canvas.setBackgroundImage("HexBack.png");
	
	int center = _controller.window.getSize().x/2; 
	_canvas.addTextbox(TEAM_NAME_TEXTBOX_ID).setPosition(center-185, 315);
	_canvas.addButton<GraphicUserManager>(
		&GraphicUserManager::submitTeamNameForm, this, "Register"
	).setPosition(center+75, 315);
	
	redrawCanvas();
}

/**
  * Method handling the main menu interface
  */
void GraphicUserManager::displayMainMenu()
{
	clear();

	addTopBar(user());

	_canvas.setBackgroundImage("HexBack.png");
	
	_canvas.addButton<GraphicUserManager>(
		&GraphicUserManager::goToPlayers, this, "Team management"
	).setPosition(100, 250);

	_canvas.addButton<GraphicUserManager>(
		&GraphicUserManager::goToStadium, this, "Stadium management"
	).setPosition(100, 300);

	_canvas.addButton<GraphicUserManager>(
		&GraphicUserManager::goToMarket, this, "Player market"
	).setPosition(100, 350);

	_canvas.addButton<GraphicUserManager>(
		&GraphicUserManager::goToFriendlyGame, this, "Play a friendly game"
	).setPosition(100, 400);

	_canvas.addButton<GraphicUserManager>(
		&GraphicUserManager::goToChampionships, this, "Championships"
	).setPosition(100, 450);

	addBackButton("Exit").setPosition(100, 550);
	
	redrawCanvas();
}

/******* BUTTONS CALLBACKS ******/
/**
  * Method handling the login service
  */
void GraphicUserManager::submitLoginForm()
{
	GUI::Textbox & userTextbox = _canvas.textboxWithID(USERNAME_TEXTBOX_ID);
	GUI::Textbox & passTextbox = _canvas.textboxWithID(PASSWORD_TEXTBOX_ID);
	_wait = true;	
	loginUser(userTextbox.getText(), passTextbox.getText());
	while (_wait){
		readEvent();
		readMessages();
	}
}

/**
  * Method handling the registartion service
  */
void GraphicUserManager::submitRegisterForm()
{
	GUI::Textbox & userTextbox = _canvas.textboxWithID(USERNAME_TEXTBOX_ID);
	GUI::Textbox & passTextbox = _canvas.textboxWithID(PASSWORD_TEXTBOX_ID);
	GUI::Textbox & confTextbox = _canvas.textboxWithID(NEW_PASSWORD_CONFIRMATION_TEXTBOX_ID);

	if (passTextbox.getText() != confTextbox.getText()){
		onRegisterUserError("The 2 passwords doesn't match");
	} else {
		_wait = true;
		registerUser(userTextbox.getText(), passTextbox.getText());
		while (_wait){
			readEvent();
			readMessages();
		}
	}
}

/**
  * Method
  */
void GraphicUserManager::submitTeamNameForm()
{
	GUI::Textbox & textbox = _canvas.textboxWithID(TEAM_NAME_TEXTBOX_ID);
	_wait = true;
	chooseTeamName(user().username, textbox.getText());
	while (_wait){
		readEvent();
		readMessages();
	}
}

/**
  * Method handling the stadium interface
  */
void GraphicUserManager::goToStadium()
{
	GraphicStadiumManager stadium(*this, _controller);
	stadium.run();
	deleteCanvas();
	displayMainMenu();
}

/**
  * Method handling the market interface
  */
void GraphicUserManager::goToMarket()
{
	GraphicMarketManager market(*this, _controller);
	market.run();
	deleteCanvas();
	displayMainMenu();
}

/**
  * Method handling the friendly game interface
  */
void GraphicUserManager::goToFriendlyGame()
{
	GUIFGameManager game(*this, _controller);
	game.run();
	deleteCanvas();
	displayMainMenu();
}

/**
  * Method handling the player management interface
  */
void GraphicUserManager::goToPlayers()
{
	GraphicTeamManager market(*this, _controller);
	market.run();
	deleteCanvas();
	displayMainMenu();
}

/**
  * Method handling the championship interface
  */
void GraphicUserManager::goToChampionships()
{
	GraphicChampionshipManager champ(*this, _controller);
	champ.run();
	deleteCanvas();
	displayMainMenu();
}


/******* HOOKS *******/

/**
  * Method handling a valid login
  */
void GraphicUserManager::onLoginOK()
{
	displayMainMenu();
	_wait = false;
}

/**
  * Method handling a login error
  * @param string :  error message to be displayed
  */
void GraphicUserManager::onLoginError(std::string const & err)
{
	displayChoice();
	displayError(err);
	_wait = false;
}

/**
  * Method handling valid team credentials
  */
void GraphicUserManager::onTeamNameOK()
{
	onLoginOK();
}

/**
  * Method handling team credentials error
  * @param string : error message to be displayed
  */
void GraphicUserManager::onTeamNameError(std::string const & err)
{
	displayError(err);
	_wait = false;
}

/**
  * Method handling a valid registration
  */
void GraphicUserManager::onRegisterUserOK()
{
	displayLoginForm();
	_wait = false;
}

/**
  * Method handling an invalid registration
  * @param string : error message to be displayed 
  */
void GraphicUserManager::onRegisterUserError(std::string const & err)
{
	displayError(err);
	_wait = false;
}

/**
  * Method handling team credentials setup
  */
void GraphicUserManager::onAskTeamName()
{
	displayTeamNameForm();
}
