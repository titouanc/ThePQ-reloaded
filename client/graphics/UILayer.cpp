#include "UILayer.hpp"
#include "SFML/Window.hpp"

using namespace std;

UILayer::~UILayer(){
	_clickables.clear();
}

void UILayer::renderTo(sf::RenderTarget & dest){
	dest.clear(_backgroundColor);
	for(unsigned int i=0; i<_clickables.size(); ++i)
		_clickables[i]->renderTo(dest);
}

void UILayer::handleClick(int x, int y){
	for (unsigned int i=0; i<_clickables.size(); ++i){
		if (_clickables[i]->isInBounds(x, y))
			_clickables[i]->triggerAction();
	}
}

