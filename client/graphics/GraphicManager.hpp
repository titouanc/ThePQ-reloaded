#ifndef GRAPHIC_MANAGER_HPP
#define GRAPHIC_MANAGER_HPP

#include "MainController.hpp"
#include "Layer.hpp"
#include "Button.hpp"
#include "Textbox.hpp"
#include "Label.hpp"

namespace GUI {
	class GraphicManager {
	public:
		GraphicManager(MainController &uic) : _controller(uic), _isRunning(true){}
		void run();
		void deleteCanvas();
		void displayCanvas();
		void redrawCanvas();
	protected:
		MainController &_controller;
		Layer _canvas;
		bool _isRunning;
	};
}

#endif