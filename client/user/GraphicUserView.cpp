#include "GraphicUserView.hpp"
#include <stadium/GraphicStadiumView.hpp>
#include <market/GraphicMarketView.hpp>
#include <fgame/GraphicFGameView.hpp>
#include <team/GraphicTeamView.hpp>
#include <championship/GraphicChampionshipView.hpp>

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
GraphicUserView::GraphicUserView(
	    net::ClientConnectionManager& connection, 
    	UserData& user, 
    	std::queue<JSON::Dict> & notifications, 
    	GUI::MainController &controller
	) : 
    UserController(connection, user, notifications), 
    GraphicManager(controller)
{
	_canvas.setBackgroundImage("SplashScreenBG.png");
	displayCanvas();
	displayChoice();
}

 /**
  * Method handling the registration interface
  */
void GraphicUserView::displayChoice()
{
	clear();

	Button<GraphicUserView> & loginButton = _canvas.addButton<GraphicUserView>(
		&GraphicUserView::displayLoginForm, this, "Login");
	loginButton.setPosition(900, 350);
	loginButton.setBackgroundColor(DARK_BUTTON_BACKGROUND_COLOR);

	Button<GraphicUserView> & registerButton = _canvas.addButton<GraphicUserView>(
		&GraphicUserView::displayRegisterForm, this, "Register");
	registerButton.setPosition(900, 410);
	registerButton.setBackgroundColor(DARK_BUTTON_BACKGROUND_COLOR);
	
	redrawCanvas();
}

/**
  * Method handling the login form interface
  */
void GraphicUserView::displayLoginForm()
{
	clear();
	
	_canvas.addTextbox(USERNAME_TEXTBOX_ID).setPosition(900, 300);
	_canvas.addTextbox(PASSWORD_TEXTBOX_ID).setPosition(900, 360);
	Button<GraphicUserView> & loginButton = _canvas.addButton<GraphicUserView>(
		&GraphicUserView::submitLoginForm, this, "Login");
	loginButton.setPosition(900, 420);
	loginButton.setBackgroundColor(DARK_BUTTON_BACKGROUND_COLOR);
	
	redrawCanvas();
}

/**
  * Method handling the registration interface
  */
void GraphicUserView::displayRegisterForm()
{
	clear();
	
	_canvas.addTextbox(USERNAME_TEXTBOX_ID).setPosition(900, 300);
	_canvas.addTextbox(PASSWORD_TEXTBOX_ID).setPosition(900, 360);
	_canvas.addTextbox(NEW_PASSWORD_CONFIRMATION_TEXTBOX_ID).setPosition(900, 420);
	Button<GraphicUserView> & registerButton = _canvas.addButton<GraphicUserView>(
		&GraphicUserView::submitRegisterForm, this, "Register");
	registerButton.setPosition(900, 480);
	registerButton.setBackgroundColor(DARK_BUTTON_BACKGROUND_COLOR);
	
	redrawCanvas();
}

/**
  * Method handling the team configuration interface
  */
void GraphicUserView::displayTeamNameForm()
{
	clear();

	_canvas.setBackgroundImage("HexBack.png");
	
	int center = _controller.window.getSize().x/2; 
	_canvas.addTextbox(TEAM_NAME_TEXTBOX_ID).setPosition(center-185, 315);
	_canvas.addButton<GraphicUserView>(
		&GraphicUserView::submitTeamNameForm, this, "Register"
	).setPosition(center+75, 315);
	
	redrawCanvas();
}

/**
  * Method handling the main menu interface
  */
void GraphicUserView::displayMainMenu()
{
	clear();

	addTopBar(user());

	_canvas.setBackgroundImage("HexBack.png");
	
	_canvas.addButton<GraphicUserView>(
		&GraphicUserView::goToPlayers, this, "Team management"
	).setPosition(100, 250);

	_canvas.addButton<GraphicUserView>(
		&GraphicUserView::goToStadium, this, "Stadium management"
	).setPosition(100, 300);

	_canvas.addButton<GraphicUserView>(
		&GraphicUserView::goToMarket, this, "Player market"
	).setPosition(100, 350);

	_canvas.addButton<GraphicUserView>(
		&GraphicUserView::goToFriendlyGame, this, "Play a friendly game"
	).setPosition(100, 400);

	_canvas.addButton<GraphicUserView>(
		&GraphicUserView::goToChampionships, this, "Championships"
	).setPosition(100, 450);

	_canvas.addButton<GraphicUserView>(
		&GraphicUserView::displayCredits, this, "Credits"
	).setPosition(1000, 500);

	addBackButton("Exit").setPosition(1000, 550);
	
	redrawCanvas();
}

/******* BUTTONS CALLBACKS ******/
void GraphicUserView::displayCredits()
{
	displayMessage(
		"The Pro Quidditch Manager\n"
		"Anthony Caccia, Antoine Carpentier, Titouan Christophe,\n"
		"Jerome Hellinckx, Florentin Hennecker, Mircea Mudura\n"
		"Music: \n"
		"   Quidditch Futura by MattendrizZ\n"
		"   Marvelous Space Adventures by jANUS",
		{"close"}
	);
}

/**
  * Method handling the login service
  */
void GraphicUserView::submitLoginForm()
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
void GraphicUserView::submitRegisterForm()
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
void GraphicUserView::submitTeamNameForm()
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
void GraphicUserView::goToStadium()
{
	GraphicStadiumView stadium(*this, _controller);
	stadium.run();
	deleteCanvas();
	displayMainMenu();
}

/**
  * Method handling the market interface
  */
void GraphicUserView::goToMarket()
{
	GraphicMarketView market(*this, _controller);
	market.run();
	deleteCanvas();
	displayMainMenu();
}

/**
  * Method handling the friendly game interface
  */
void GraphicUserView::goToFriendlyGame()
{
	GraphicFGameView game(*this, _controller);
	game.run();
	deleteCanvas();
	displayMainMenu();
}

/**
  * Method handling the player management interface
  */
void GraphicUserView::goToPlayers()
{
	GraphicTeamView market(*this, _controller);
	market.run();
	deleteCanvas();
	displayMainMenu();
}

/**
  * Method handling the championship interface
  */
void GraphicUserView::goToChampionships()
{
	GraphicChampionshipView champ(*this, _controller);
	champ.run();
	deleteCanvas();
	displayMainMenu();
}


/******* HOOKS *******/

/**
  * Method handling a valid login
  */
void GraphicUserView::onLoginOK()
{
	displayMainMenu();
	_wait = false;
}

/**
  * Method handling a login error
  * @param string :  error message to be displayed
  */
void GraphicUserView::onLoginError(std::string const & err)
{
	displayChoice();
	displayError(err);
	_wait = false;
}

/**
  * Method handling valid team credentials
  */
void GraphicUserView::onTeamNameOK()
{
	onLoginOK();
}

/**
  * Method handling team credentials error
  * @param string : error message to be displayed
  */
void GraphicUserView::onTeamNameError(std::string const & err)
{
	displayError(err);
	_wait = false;
}

/**
  * Method handling a valid registration
  */
void GraphicUserView::onRegisterUserOK()
{
	displayLoginForm();
	_wait = false;
}

/**
  * Method handling an invalid registration
  * @param string : error message to be displayed 
  */
void GraphicUserView::onRegisterUserError(std::string const & err)
{
	displayError(err);
	_wait = false;
}

/**
  * Method handling team credentials setup
  */
void GraphicUserView::onAskTeamName()
{
	displayTeamNameForm();
}
