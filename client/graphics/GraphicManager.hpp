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
	protected:
		sf::RenderWindow & window(){return _controller.window;}
		MainController &_controller;
		Layer _canvas;
		bool _isRunning;
	};
}

#endif