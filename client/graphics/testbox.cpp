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
		myTextbox = _canvas.addTextbox("superwow");
		myTextbox->setPosition(10, 60);
		Button<TestBoxManager> *myButton = _canvas.addButton<TestBoxManager>(&TestBoxManager::printWowTextBox, this, "Cout Wow Box");
		myButton->setPosition(270, 60);
		displayCanvas();
		run();
	}

	void printWowTextBox(){
		cout << "directly accessed : " << myTextbox->getText() << endl;
	}

private:
	Textbox *myTextbox;
	
};

int main(int argc, char const *argv[])
{
	MainController controller;
	TestBoxManager tbm(controller);
	return 0;
}