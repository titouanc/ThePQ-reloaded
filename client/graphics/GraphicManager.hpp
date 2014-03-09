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
		virtual void run();
		void deleteCanvas();
		void displayCanvas();
		void redrawCanvas();
		void stop(){_isRunning = false;};
	protected:
		/* Standard event processing; always return true */
		virtual bool treatEvent(sf::Event const & event);
		/* non blocking event pop+treat. If no event popped, return false; */
		bool readEvent();

		/* Create and add a button bound to stop() then return it */
		GUI::Button<GraphicManager> & backButton(std::string const & caption="Back");

		sf::RenderWindow & window(){return _controller.window;}
		MainController &_controller;
		Layer _canvas;
		bool _isRunning;
	};
}

#endif