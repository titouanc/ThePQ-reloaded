#ifndef GRAPHIC_MANAGER_HPP
#define GRAPHIC_MANAGER_HPP

#include "MainController.hpp"
#include "Layer.hpp"
#include "Button.hpp"
#include "Textbox.hpp"
#include "Label.hpp"
#include "TableView.hpp"

namespace GUI {
	class GraphicManager {
	public:
		GraphicManager(MainController &uic) : _controller(uic), _isRunning(true){}
		
		/*! run the GraphicManager mainloop */
		virtual void run();

		/*! hook called at every mainloop iteration (same as event loop !!!) */
		virtual void loop(){}

		void deleteCanvas();
		void displayCanvas();
		void redrawCanvas();
		void stop(){_isRunning = false;}

		void displayError(std::string errorMessage);
		void displayMessage(std::string message);
		int confirm(std::string message);
		void doNothing(){}
	protected:
		/* Standard event processing; always return true */
		virtual bool treatEvent(sf::Event const & event);
		/* non blocking event pop+treat. If no event popped, return false; */
		bool readEvent();

		/* Create and add a button bound to stop() then return it */
		GUI::Button<GraphicManager> & backButton(std::string const & caption="Back");
		GUI::Button<GraphicManager> & usernameButton(std::string const username);
		GUI::Button<GraphicManager> & userBudgetButton(const int budget);	
		GUI::Button<GUI::GraphicManager> & userAcPointsButton(const int acPoints);	
		GUI::Button<GUI::GraphicManager> & userFameButton(const int fame);
		sf::RenderWindow & window(){return _controller.window;}
		MainController &_controller;
		Layer _canvas;
		bool _isRunning;
	};
}

#endif