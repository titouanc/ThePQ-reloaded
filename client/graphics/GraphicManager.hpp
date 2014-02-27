#include "MainController.hpp"
#include "Layer.hpp"
#include "Button.hpp"
#include "Textbox.hpp"

namespace GUI {
	class GraphicManager {
	public:
		GraphicManager(MainController &uic) : _controller(uic), _isRunning(true){}
		void run();
		void deleteCanvas();
		void displayCanvas();
	protected:
		MainController &_controller;
		Layer _canvas;
		bool _isRunning;
	};
}