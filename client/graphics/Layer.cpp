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
	map<string, Textbox*>::iterator it = _textboxes.begin();
	for(; it != _textboxes.end(); it++)
		it->second->renderTo(dest);
}

void GUI::Layer::handleClick(int x, int y){
	for (unsigned int i=0; i<_clickables.size(); ++i){
		if (_clickables[i]->isInBounds(x, y))
			_clickables[i]->triggerAction();
	}
	// checking for textboxes
	bool hasTextboxBeenSelected = false;
	map<string, Textbox*>::iterator it = _textboxes.begin();
	for(; it != _textboxes.end(); it++){
		if (it->second->isInBounds(x, y)){
			hasTextboxBeenSelected = true;
			_focusedTextbox = it->second;
			it->second->focus();
		}
	}
	if (!hasTextboxBeenSelected)
		_focusedTextbox = NULL;
}

void GUI::Layer::handleTextEntered(sf::Event event){
	if (_focusedTextbox != NULL)
		_focusedTextbox->updateText(event);
}

GUI::Textbox* GUI::Layer::addTextbox(string id){
	Textbox* res = new Textbox();

	_textboxes.insert(pair<string, Textbox*>(id, res));
	return res;
}

GUI::Textbox* GUI::Layer::textboxWithID(string id){
	return _textboxes[id];
}

void GUI::Layer::unfocusAllTextboxes(){
	map<string, Textbox*>::iterator it = _textboxes.begin();
	for(; it != _textboxes.end(); it++)
		it->second->unfocus();
}

