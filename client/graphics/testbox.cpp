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
		_canvas.addTextbox("such");
		myTextbox = _canvas.addTextbox("wow");
		myTextbox->setPosition(10, 60);

		Button<TestBoxManager> *myButton = _canvas.addButton<TestBoxManager>(&TestBoxManager::printWowTextBox, this, "Cout Wow Box");
		myButton->setPosition(270, 60);

		hideButton = _canvas.addButton<TestBoxManager>(&TestBoxManager::hideTextbox, this, "Hide Textbox");
		hideButton->setPosition(10, 110);

		normalLabel = _canvas.addLabel("This is a label.");
		normalLabel->setPosition(500, 10);

		amazingLabel = _canvas.addLabel("This is a\ngreat label!", sf::Color(0xff, 0x00, 0x00, 0xff));
		amazingLabel->setPosition(500, 60);

		displayCanvas();
		run();
	}

	void printWowTextBox(){
		cout << "directly accessed : \"" << myTextbox->getText() << "\"" << endl;
		cout << "searched from id  : \"" << _canvas.textboxWithID("wow")->getText() << "\"" << endl;
	}

	void hideTextbox(){
		if (myTextbox->isHidden()){
			myTextbox->unhide();
			hideButton->setText("Hide Textbox");
		}
		else {
			myTextbox->hide();
			hideButton->setText("Unhide Textbox");
		}
		redrawCanvas();
	}

private:
	Textbox *myTextbox;
	Button<TestBoxManager> *hideButton;
	Label *normalLabel;
	Label *amazingLabel;
	
};

int main(int argc, char const *argv[])
{
	MainController controller;
	TestBoxManager tbm(controller);
	return 0;
}