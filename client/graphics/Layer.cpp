#include "Layer.hpp"
#include "SFML/Window.hpp"

using namespace std;
using namespace GUI;

GUI::Layer::~Layer(){
	_clickables.clear();
	_textboxes.clear();
	_labels.clear();
}

void GUI::Layer::renderTo(sf::RenderTarget & dest){
	dest.clear(_backgroundColor);
	// rendering clickables
	for(unsigned int i=0; i<_clickables.size(); ++i)
		if (!_clickables[i]->isHidden())
			_clickables[i]->renderTo(dest);
	// rendering textboxes
	map<string, Textbox*>::iterator it = _textboxes.begin();
	for(; it != _textboxes.end(); it++)
		if (!it->second->isHidden())
			it->second->renderTo(dest);
	// rendering labels
	for(unsigned i=0; i<_labels.size(); ++i){
		if (!_labels[i]->isHidden())
			_labels[i]->renderTo(dest);
	}
}

void GUI::Layer::handleClick(int x, int y){
	for (unsigned int i=0; i<_clickables.size(); ++i){
		if (_clickables[i]->isInBounds(x, y) && !_clickables[i]->isHidden())
			_clickables[i]->triggerAction();
	}
	// checking for textboxes
	bool hasTextboxBeenSelected = false;
	map<string, Textbox*>::iterator it = _textboxes.begin();
	for(; it != _textboxes.end(); it++){
		if (it->second->isInBounds(x, y) && !it->second->isHidden()){
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
	Textbox* res = new Textbox(id);

	_textboxes.insert(pair<string, Textbox*>(id, res));
	return res;
}

GUI::Label* GUI::Layer::addLabel(std::string text, sf::Color color){
	Label* res = new Label(text, color);
	_labels.push_back(res);
	return res;
}

GUI::Textbox* GUI::Layer::textboxWithID(string id){
	if (_textboxes[id] == NULL)
		throw "There is no textbox with id " + id + ".";
	return _textboxes[id];
}

void GUI::Layer::unfocusAllTextboxes(){
	map<string, Textbox*>::iterator it = _textboxes.begin();
	for(; it != _textboxes.end(); it++)
		it->second->unfocus();
}

