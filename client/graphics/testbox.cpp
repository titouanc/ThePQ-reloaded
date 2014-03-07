#include <iostream>
#include <string>
#include <stdlib.h> // itoa

#include "GraphicManager.hpp"
//#include "MessageBox.hpp"

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
		
		TableView *myTableView = _canvas.addTableView(2);
		for (int i = 0; i < 5; ++i)
		{
			TableCell* myTableCell = new TableCell(100, 100, sf::Color(0xee,0xee,0xee,0xff));
			switch (i){
				case 0:
					myTableCell->addLabel("Wow");
					break;
				case 1:
					myTableCell->addLabel("Dat");
					break;
				case 2:
					myTableCell->addLabel("TableView");
					break;
				case 3:
					myTableCell->addLabel("Such");
					break;
				case 4:
					myTableCell->addLabel("Magic");
					break;
			}
			Button<TestBoxManager>* b = myTableCell->addButton<TestBoxManager>(&TestBoxManager::printWow, this, "Wow");
			b->setPosition(10, 40);
			myTableView->append(myTableCell);
		}

		myTableView->setPosition(10, 400);

		displayCanvas();
		run();
	}

	void printWow(){
		cout << "wow"<< endl;
	}

	void printWowTextBox(){
		cout << "directly accessed : \"" << myTextbox->getText() << "\"" << endl;
		cout << "searched from id  : \"" << _canvas.textboxWithID("wow")->getText() << "\"" << endl;
		//MessageBox mb(_controller, "Je code avec le cul, lalalalala.");
		//mb.run();
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