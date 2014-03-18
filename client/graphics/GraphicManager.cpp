#include "GraphicManager.hpp"
#include "MessageBox.hpp"
#include <sstream>

void GUI::GraphicManager::run()
{
	while(_controller.window.isOpen() && _isRunning){
		sf::Event event;
		if (_controller.window.pollEvent(event))
			treatEvent(event);
		loop();
	}
}

bool GUI::GraphicManager::readEvent()
{
	sf::Event event;
	if (! _controller.window.pollEvent(event))
		return false;
	treatEvent(event);
	return true;
}

bool GUI::GraphicManager::treatEvent(sf::Event const & event)
{
	if (event.type == sf::Event::GainedFocus){
		redrawCanvas();
	}
	else if (event.type == sf::Event::MouseButtonPressed){
		if (event.mouseButton.button == sf::Mouse::Left)
			_controller.handleClick(event);
		else if (event.mouseButton.button == sf::Mouse::Right)
			_controller.handleRightClick(event);
	}
	else if (event.type == sf::Event::TextEntered){
		_controller.handleTextEntered(event);
	}
	else if (event.type == sf::Event::MouseMoved){
		_controller.handleMouseMoved(event);
	}
	else if (event.type == sf::Event::Closed ||
			(event.type==sf::Event::KeyPressed 
			&& event.key.code==sf::Keyboard::Escape)){
		window().close();
	}
	return true;
}

void GUI::GraphicManager::deleteCanvas()
{
	//_isRunning = false;
	_controller.deleteTopLayer();
}

void GUI::GraphicManager::displayCanvas()
{
	_controller.addLayer(_canvas);
}

void GUI::GraphicManager::clear()
{
	_canvas.clear();
	_backButton = NULL;
	_usernameButton = NULL;
	_userBudgetButton = NULL;
	_userAcPointsButton = NULL;
	_userFameButton = NULL;
}

void GUI::GraphicManager::redrawCanvas()
{
	_canvas.renderTo(_controller.window);
	window().display();
}

void GUI::GraphicManager::displayError(std::string errorMessage){
	MessageBox m(_controller, "Error : "+errorMessage, {"OK"});
	m.showBox();
}

void GUI::GraphicManager::displayOk(std::string okMessage){
	MessageBox m(_controller,"OK : "+okMessage, {"YAY !"});
	m.showBox();
}

void GUI::GraphicManager::displayMessage(std::string message){
	MessageBox m(_controller, message, {"OK"});
	m.showBox();
}

int GUI::GraphicManager::confirm(std::string message){
	MessageBox m(_controller, message, {"No", "Yes"});
	return m.showBox();
}

void GUI::GraphicManager::updateBudget(int newBudget){
	std::stringstream s;
	s << newBudget << " $";
	if (_userBudgetButton)
		_userBudgetButton->setText(s.str());
	positionTopButtons();
}

void GUI::GraphicManager::updateAcPoints(int newAcPoints){
	std::stringstream s;
	s << newAcPoints << " AP";
	if (_userAcPointsButton)
		_userAcPointsButton->setText(s.str());
	positionTopButtons();
}

void GUI::GraphicManager::updateFame(int newFame){
	std::stringstream s;
	s << "Fame : " << newFame;
	if (_userFameButton)
		_userFameButton->setText(s.str());
	positionTopButtons();
}

void GUI::GraphicManager::updateUsername(std::string username){
	if (_usernameButton)
		_usernameButton->setText("Welcome, "+username+" !");
}

void GUI::GraphicManager::positionTopButtons(){
	int totalLength = GUI::MARGIN;
	if (_userBudgetButton){
		_userBudgetButton->setPosition(window().getSize().x-totalLength-_userBudgetButton->getWidth(), MARGIN);
		totalLength = totalLength + _userBudgetButton->getWidth() + MARGIN;
	}
	if (_userFameButton){
		_userFameButton->setPosition(window().getSize().x-totalLength-_userFameButton->getWidth(), MARGIN);
		totalLength = totalLength + _userFameButton->getWidth() + MARGIN;
	}
	if (_userAcPointsButton){
		_userAcPointsButton->setPosition(window().getSize().x-totalLength-_userAcPointsButton->getWidth(), MARGIN);
		totalLength = totalLength + _userAcPointsButton->getWidth() + MARGIN;
	}
}


GUI::Button<GUI::GraphicManager> & GUI::GraphicManager::addBackButton(std::string const & caption)
{
	_backButton = &_canvas.addButton<GUI::GraphicManager>(
		&GUI::GraphicManager::stop, this, caption
	);
	_backButton->setPosition(window().getSize().x-_backButton->getWidth()-MARGIN, window().getSize().y-_backButton->getHeight()-MARGIN);
	return *_backButton;
}

GUI::Button<GUI::GraphicManager> & GUI::GraphicManager::addUsernameButton(std::string const username)
{
	_usernameButton = &_canvas.addButton<GUI::GraphicManager>(
		&GUI::GraphicManager::doNothing, this, "Welcome, "+username+"!"
	);
	updateUsername(username);
	_usernameButton->disable();
	_usernameButton->setBackgroundColor(LIGHT_BUTTON_BACKGROUND_COLOR);
	_usernameButton->setTextColor(LIGHT_BUTTON_TEXT_COLOR);
	_usernameButton->setPosition(MARGIN, MARGIN);
	return *_usernameButton;
}

GUI::Button<GUI::GraphicManager> & GUI::GraphicManager::addUserBudgetButton(const int budget)
{
	_userBudgetButton = &_canvas.addButton<GUI::GraphicManager>(
		&GUI::GraphicManager::doNothing, this, "0"
	);
	updateBudget(budget);
	_userBudgetButton->disable();
	_userBudgetButton->setBackgroundColor(LIGHT_BUTTON_BACKGROUND_COLOR);
	_userBudgetButton->setTextColor(LIGHT_BUTTON_TEXT_COLOR);
	_userBudgetButton->setPosition(window().getSize().x-_userBudgetButton->getWidth()-MARGIN, MARGIN);
	return *_userBudgetButton;
}

GUI::Button<GUI::GraphicManager> & GUI::GraphicManager::addUserAcPointsButton(const int acPoints)
{
	_userAcPointsButton = &_canvas.addButton<GUI::GraphicManager>(
		&GUI::GraphicManager::doNothing,this,"0"
		);
	updateAcPoints(acPoints);
	_userAcPointsButton->disable();
	_userAcPointsButton->setBackgroundColor(LIGHT_BUTTON_BACKGROUND_COLOR);
	_userAcPointsButton->setTextColor(LIGHT_BUTTON_TEXT_COLOR);
	_userAcPointsButton->setPosition(window().getSize().x-_userAcPointsButton->getWidth()*1.8-1.2*MARGIN,MARGIN);
	return *_userAcPointsButton;
}

GUI::Button<GUI::GraphicManager> & GUI::GraphicManager::addUserFameButton(const int fame)
{
	_userFameButton = &_canvas.addButton<GUI::GraphicManager>(
		&GUI::GraphicManager::doNothing,this,"0"
		);
	updateFame(fame);
	_userFameButton->disable();
	_userFameButton->setBackgroundColor(LIGHT_BUTTON_BACKGROUND_COLOR);
	_userFameButton->setTextColor(LIGHT_BUTTON_TEXT_COLOR);
	_userFameButton->setPosition(window().getSize().x-_userFameButton->getWidth()*2.8-3.5*MARGIN,MARGIN);
	return *_userFameButton;
}

void GUI::GraphicManager::addTopBar(UserData & user){
	addUsernameButton(user.username);
	addUserBudgetButton(user.funds);
	addUserFameButton(user.fame);
	addUserAcPointsButton(user.acPoints);
	positionTopButtons();
}
