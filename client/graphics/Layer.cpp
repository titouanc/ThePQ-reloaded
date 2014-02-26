#include "Layer.hpp"
#include "SFML/Window.hpp"

using namespace std;
using namespace GUI;

GUI::Layer::~Layer(){
	_clickables.clear();
}

void GUI::Layer::renderTo(sf::RenderTarget & dest){
	dest.clear(_backgroundColor);
	for(unsigned int i=0; i<_clickables.size(); ++i)
		_clickables[i]->renderTo(dest);
}

void GUI::Layer::handleClick(int x, int y){
	for (unsigned int i=0; i<_clickables.size(); ++i){
		if (_clickables[i]->isInBounds(x, y))
			_clickables[i]->triggerAction();
	}
}

