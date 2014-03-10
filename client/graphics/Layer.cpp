#include "Layer.hpp"
#include "TableView.hpp"
#include "SFML/Window.hpp"
#include <sstream>
#include <iostream>

using namespace std;
using namespace GUI;

GUI::Layer::~Layer(){
	clear();
}

void GUI::Layer::clear(){
	std::map<std::string, GUI::Textbox*>::iterator it;
	for (it=_textboxes.begin(); it!=_textboxes.end(); it++)
		delete it->second;
	for (size_t i=0; i<_clickables.size(); i++)
		delete _clickables[i];
	for (size_t i=0; i<_labels.size(); i++)
		delete _labels[i];
	for (size_t i=0; i<_tableCells.size(); i++)
		delete _tableCells[i];
	for (size_t i=0; i<_tableViews.size(); i++)
		delete _tableViews[i];
	_clickables.clear();
	_textboxes.clear();
	_labels.clear();
	_tableCells.clear(); // TODO delete
	_tableViews.clear();
}

void GUI::Layer::renderTo(sf::RenderTarget & dest){
	if (_backgroundColor.a != 0xff){
		sf::RectangleShape background(sf::Vector2f(dest.getSize().x, dest.getSize().y));
		background.setFillColor(_backgroundColor);
		dest.draw(background);
	}
	else
		dest.clear(_backgroundColor);
	renderAllAttributesTo(dest);
}

void GUI::Layer::renderAllAttributesTo(sf::RenderTarget &dest){
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
	for(unsigned i=0; i<_tableCells.size(); ++i){ // TODO delete
		if(!_tableCells[i]->isHidden())
			_tableCells[i]->renderTo(dest);
	}
	for(unsigned i=0; i<_tableViews.size(); ++i){
		if(!_tableViews[i]->isHidden())
			_tableViews[i]->renderTo(dest);
	}

}

bool GUI::Layer::handleClick(int x, int y){
	for (unsigned int i=0; i<_clickables.size(); ++i){
		if (_clickables[i]->isInBounds(x, y) && !_clickables[i]->isHidden()){
			_clickables[i]->triggerAction();
			return true;
		}	
	}
	// checking for textboxes
	bool hasTextboxBeenSelected = false;
	map<string, Textbox*>::iterator it = _textboxes.begin();
	for(; it != _textboxes.end(); it++){
		if (it->second->isInBounds(x, y) && !it->second->isHidden()){
			hasTextboxBeenSelected = true;
			_focusedTextbox = it->second;
			it->second->focus();
			return true;
		}
	}
	if (!hasTextboxBeenSelected)
		_focusedTextbox = NULL;

	for (unsigned int i=0; i<_tableViews.size(); ++i){
		if (!_tableViews[i]->isHidden()){
			_tableViews[i]->handleClick(x,y);
			return true;
		}
	}
	return false;
}

bool GUI::Layer::handleRightClick(int x, int y){
	// delete text in a textbox
	bool hasTextboxBeenSelected = false;
	map<string, Textbox*>::iterator it = _textboxes.begin();
	for(; it != _textboxes.end(); it++){
		if (it->second->isInBounds(x, y) && !it->second->isHidden()){
			hasTextboxBeenSelected = true;
			_focusedTextbox = it->second;
			it->second->focus();
			it->second->clearText();
			return true;
		}
	}
	if (!hasTextboxBeenSelected)
		_focusedTextbox = NULL;
	return false;
}

void GUI::Layer::handleTextEntered(sf::Event event){
	if (_focusedTextbox != NULL)
		_focusedTextbox->updateText(event);
}

GUI::Textbox & GUI::Layer::addTextbox(string id){
	Textbox* res = new Textbox(id);

	_textboxes.insert(pair<string, Textbox*>(id, res));
	return *res;
}

GUI::Label & GUI::Layer::addLabel(std::string text, sf::Color color){
	Label* res = new Label(text, color);
	_labels.push_back(res);
	return *res;
}

GUI::Label & GUI::Layer::addLabel(double val, sf::Color color){
	std::stringstream converter;
	converter << val;
	return addLabel(converter.str());
}

GUI::TableCell & GUI::Layer::addTableCell(){
	TableCell* res = new TableCell();
	_tableCells.push_back(res);
	return *res;
}

GUI::TableView & GUI::Layer::addTableView(int columns, int padding){
	TableView* res = new TableView(columns, padding);
	_tableViews.push_back(res);
	return *res;
}

GUI::Textbox & GUI::Layer::textboxWithID(string id){
	if (_textboxes[id] == NULL)
		throw "There is no textbox with id " + id + ".";
	return *_textboxes[id];
}

void GUI::Layer::unfocusAllTextboxes(){
	map<string, Textbox*>::iterator it = _textboxes.begin();
	for(; it != _textboxes.end(); it++)
		it->second->unfocus();
}

