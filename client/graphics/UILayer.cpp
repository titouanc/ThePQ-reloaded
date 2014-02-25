#include "UILayer.hpp"

void UILayer::handleClick(int x, int y){
	for (int i=0; i<_clickables.size(); ++i){
		if (_clickables[i].isInBounds(x, y)
			_clickables[i].triggerAction();
	}
}