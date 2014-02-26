#include <iostream>
#include <string>

#include "GraphicManager.hpp"

using namespace std;
using namespace GUI;

class GraphicManager2 : public GraphicManager {
public:
	GraphicManager2(MainController &uic) : GraphicManager(uic){
		Button<GraphicManager2> *button = _canvas.addButton<GraphicManager2>(&GraphicManager2::superMethod, this, "gm 2");
		button->setPosition(100, 100);
		displayCanvas();
		run();
	}
	~GraphicManager2(){}

	void superMethod() { 
		cout << "deleting gm2" << endl; 
		deleteCanvas();
	}

};

class GraphicManager1 : public GraphicManager {
public:
	GraphicManager1(MainController &uic) : GraphicManager(uic){
		_canvas.addButton<GraphicManager1>(&GraphicManager1::myMethod, this, "gm 1");
		displayCanvas();
		run();
	}

	void myMethod() { 
		cout << "constructing gm2!" << endl; 
		GraphicManager2 gm2(_controller);
	}
};



int main(int argc, char const *argv[])
{
	MainController controller;
	GraphicManager1 d(controller);
	return 0;
}

