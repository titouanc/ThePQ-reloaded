#include <iostream>
#include <string>

#include "GraphicManager.hpp"

using namespace std;
using namespace GUI;

class TestBoxManager : public GraphicManager {
public:
	TestBoxManager(MainController &mc):GraphicManager(mc){
		//Textbox myTextbox;
		//myTextbox.renderTo(_controller.window);
		_canvas.addTextbox("wow");
		displayCanvas();
		run();
	}
	
};

int main(int argc, char const *argv[])
{
	MainController controller;
	TestBoxManager tbm(controller);
	return 0;
}