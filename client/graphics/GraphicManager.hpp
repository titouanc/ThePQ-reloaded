#ifndef GRAPHIC_MANAGER_HPP
#define GRAPHIC_MANAGER_HPP

#include "MainController.hpp"
#include "Layer.hpp"
#include "Button.hpp"
#include "Textbox.hpp"
#include "Label.hpp"
#include "TableView.hpp"
#include "../UserData.hpp"
#include <vector>

namespace GUI {
	class GraphicManager {
	public:
		GraphicManager(MainController &uic) : _controller(uic), _isRunning(true),
		_backButton(NULL), _usernameButton(NULL), _userBudgetButton(NULL),
		_userAcPointsButton(NULL), _userFameButton(NULL) {}
		
		/*! run the GraphicManager mainloop */
		virtual void run();

		/*! hook called at every mainloop iteration (same as event loop !!!) */
		virtual void loop(){}

		void deleteCanvas();
		void displayCanvas();
		void redrawCanvas();
		void clear();
		void stop(){_isRunning = false;}

		void displayError(std::string errorMessage);
		void displayOk(std::string okMessage);
		void displayMessage(std::string message, std::vector<std::string> options);
		int confirm(std::string message);

		void updateBudget(int newBudget);
		void updateAcPoints(int newAcPoints);
		void updateFame(int newFame);
		void updateUsername(std::string username);

		void positionTopButtons();

		void doNothing(){}
	protected:
		/* Standard event processing; always return true */
		virtual bool treatEvent(sf::Event const & event);
		/* non blocking event pop+treat. If no event popped, return false; */
		bool readEvent();

		/* Create and add a button bound to stop() then return it */
		GUI::Button<GraphicManager> & addBackButton(std::string const & caption="Back");
		GUI::Button<GraphicManager> & addUsernameButton(std::string const username);
		GUI::Button<GraphicManager> & addUserBudgetButton(const int budget);	
		GUI::Button<GUI::GraphicManager> & addUserAcPointsButton(const int acPoints);	
		GUI::Button<GUI::GraphicManager> & addUserFameButton(const int fame);

		void addTopBar(UserData & user);

		sf::RenderWindow & window(){return _controller.window;}
		MainController &_controller;
		Layer _canvas;
		bool _isRunning;

		GUI::Button<GraphicManager> * _backButton;
		GUI::Button<GraphicManager> * _usernameButton;
		GUI::Button<GraphicManager> * _userBudgetButton;	
		GUI::Button<GUI::GraphicManager> * _userAcPointsButton;	
		GUI::Button<GUI::GraphicManager> * _userFameButton;
	};
}

#endif