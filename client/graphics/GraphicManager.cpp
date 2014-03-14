#include "GraphicManager.hpp"
#include "MessageBox.hpp"

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
	_usernameButton->setBackgroundColor(LIGHT_BUTTON_BACKGROUND_COLOR);
	_usernameButton->setTextColor(LIGHT_BUTTON_TEXT_COLOR);
	_usernameButton->setPosition(MARGIN, MARGIN);
	return *_usernameButton;
}

GUI::Button<GUI::GraphicManager> & GUI::GraphicManager::addUserBudgetButton(const int budget)
{
	char casted[512];
	sprintf(casted, "%d $", budget);
	_userBudgetButton = &_canvas.addButton<GUI::GraphicManager>(
		&GUI::GraphicManager::doNothing, this, casted
	);
	_userBudgetButton->setBackgroundColor(LIGHT_BUTTON_BACKGROUND_COLOR);
	_userBudgetButton->setTextColor(LIGHT_BUTTON_TEXT_COLOR);
	_userBudgetButton->setPosition(window().getSize().x-_userBudgetButton->getWidth()-MARGIN, MARGIN);
	return *_userBudgetButton;
}

GUI::Button<GUI::GraphicManager> & GUI::GraphicManager::addUserAcPointsButton(const int acPoints)
{
	char casted[512];
	sprintf(casted,"%d points",acPoints);
	_userAcPointsButton = &_canvas.addButton<GUI::GraphicManager>(
		&GUI::GraphicManager::doNothing,this,casted
		);
	_userAcPointsButton->setBackgroundColor(LIGHT_BUTTON_BACKGROUND_COLOR);
	_userAcPointsButton->setTextColor(LIGHT_BUTTON_TEXT_COLOR);
	_userAcPointsButton->setPosition(window().getSize().x-_userAcPointsButton->getWidth()*1.8-1.2*MARGIN,MARGIN);
	return *_userAcPointsButton;
}

GUI::Button<GUI::GraphicManager> & GUI::GraphicManager::addUserFameButton(const int fame)
{
	char casted[512];
	sprintf(casted,"Fame: %d",fame);
	_userFameButton = &_canvas.addButton<GUI::GraphicManager>(
		&GUI::GraphicManager::doNothing,this,casted
		);
	_userFameButton->setBackgroundColor(LIGHT_BUTTON_BACKGROUND_COLOR);
	_userFameButton->setTextColor(LIGHT_BUTTON_TEXT_COLOR);
	_userFameButton->setPosition(window().getSize().x-_userFameButton->getWidth()*2.8-3.5*MARGIN,MARGIN);
	return *_userFameButton;
}
