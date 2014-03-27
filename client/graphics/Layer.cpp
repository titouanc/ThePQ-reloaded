#include "Layer.hpp"
#include "TableView.hpp"
#include "SFML/Window.hpp"
#include <sstream>
#include <iostream>
#include <toolbox.hpp>

using namespace std;
using namespace GUI;

GUI::Layer::~Layer(){
	clear();
}

void GUI::Layer::clear(bool clearBackground){
	if (clearBackground && _backgroundImage != NULL){
		delete _backgroundImage;
		_backgroundImage = NULL;
	}
	for (size_t i=0; i<_panels.size(); ++i)
		delete _panels[i];
	std::map<std::string, GUI::Textbox*>::iterator it;
	for (it=_textboxes.begin(); it!=_textboxes.end(); it++)
		delete it->second;
	for (size_t i=0; i<_clickables.size(); i++)
		delete _clickables[i];
	for (size_t i=0; i<_labels.size(); i++)
		delete _labels[i];
	for (size_t i=0; i<_tableViews.size(); i++)
		delete _tableViews[i];
	for (size_t i=0; i<_overlayPanels.size(); i++)
		delete _overlayPanels[i];
	_panels.clear();
	_clickables.clear();
	_textboxes.clear();
	_labels.clear();
	_tableViews.clear();
	_overlayPanels.clear();
}

void GUI::Layer::renderTo(sf::RenderTarget & dest, bool background){
	if (background){
		if (_backgroundImage == NULL){
			if (_backgroundColor.a != 0xff){
				sf::RectangleShape background(sf::Vector2f(dest.getSize().x, dest.getSize().y));
				background.setFillColor(_backgroundColor);
				background.setPosition(_drawX, _drawY);
				dest.draw(background);
			}
			else
				dest.clear(_backgroundColor);
		}
		else{
			_backgroundImage->setPosition(_drawX, _drawY);
	 		dest.draw(*_backgroundImage);
		}
	}
	renderAllAttributesTo(dest);
}

void GUI::Layer::renderAllAttributesTo(sf::RenderTarget &dest){		
	for (size_t i=0; i<_panels.size(); ++i){
		int ox = _panels[i]->getPosition().x;
		int oy = _panels[i]->getPosition().y;
		_panels[i]->setPosition(ox+_drawX, oy+_drawY);
		dest.draw(*(_panels[i]));
		_panels[i]->setPosition(ox, oy); // resetting
	}
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
	// rendering table views
	for(unsigned i=0; i<_tableViews.size(); ++i){
		if(!_tableViews[i]->isHidden())
			_tableViews[i]->renderTo(dest);
	}
	// rendering clickables
	for(unsigned int i=0; i<_clickables.size(); ++i)
		if (!_clickables[i]->isHidden())
			_clickables[i]->renderTo(dest);
	for(unsigned i=0; i<_overlayPanels.size(); ++i){
		int ox = _overlayPanels[i]->getPosition().x;
		int oy = _overlayPanels[i]->getPosition().y;
		_overlayPanels[i]->setPosition(ox+_drawX, oy+_drawY);
		dest.draw(*(_overlayPanels[i]));
		_overlayPanels[i]->setPosition(ox, oy); // resetting
	}

}

bool GUI::Layer::handleClick(int x, int y){
	for (unsigned int i=0; i<_clickables.size(); ++i){
		if (_clickables[i]->isInBounds(x, y) && !_clickables[i]->isHidden()){
			sf::Sound sound;
			sound.setBuffer(SOUNDS.get("click.wav"));
			sound.play();
			minisleep(0.3);
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
			if (it->second->getText() == it->second->getID())
				it->second->clearText();
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

void GUI::Layer::handleMouseMoved(sf::Event event){
	for (unsigned int i=0; i<_clickables.size(); ++i)
		_clickables[i]->onMouseMoved(event.mouseMove.x, event.mouseMove.y);
}

void GUI::Layer::setBackgroundImage(string path){
	if (_backgroundImage != NULL)
		delete _backgroundImage;
	_backgroundImage = new sf::Sprite();
	_backgroundImageTexture.loadFromFile(path);
	_backgroundImage->setTexture(_backgroundImageTexture, true);
}

void GUI::Layer::setPosition(int x, int y){
	int dx = x-_x;
	int dy = y-_y;
	_drawX += dx; // updating drawX with new x value.
	_drawY += dy; // updating drawY with new y value.
	_x = x;
	_y = y;	
	std::map<std::string, GUI::Textbox*>::iterator it;
	for (it=_textboxes.begin(); it!=_textboxes.end(); it++)
		it->second->updateDrawPosition(dx, dy);
	for (size_t i=0; i<_clickables.size(); i++)
		_clickables[i]->updateDrawPosition(dx, dy);
	for (size_t i=0; i<_labels.size(); i++)
		_labels[i]->updateDrawPosition(dx, dy);
	for (size_t i=0; i<_tableViews.size(); i++)
		_tableViews[i]->updateDrawPosition(dx, dy);
}

void GUI::Layer::deleteBackgroundImage(){
	if (_backgroundImage != NULL){
		delete _backgroundImage;
		_backgroundImage = NULL;
	}
}

GUI::Textbox & GUI::Layer::addTextbox(string id){
	Textbox* res = new Textbox(id);
	res->updateDrawPosition(this->_x, this->_y);
	_textboxes.insert(pair<string, Textbox*>(id, res));
	return *res;
}

GUI::Label & GUI::Layer::addLabel(std::string text, sf::Color color){
	Label* res = new Label(text, color);
	res->updateDrawPosition(this->_x, this->_y);
	_labels.push_back(res);
	return *res;
}

GUI::Label & GUI::Layer::addLabel(double val, sf::Color color){
	std::stringstream converter;
	converter << val;
	return addLabel(converter.str());
}

GUI::TableView & GUI::Layer::addTableView(int columns, int padding){
	TableView* res = new TableView(columns, padding);
	res->updateDrawPosition(this->_x, this->_y);
	_tableViews.push_back(res);
	return *res;
}

sf::RectangleShape & GUI::Layer::addPanel(int w, int h, sf::Color color){
	sf::RectangleShape* res = new sf::RectangleShape(sf::Vector2f(w, h));
	res->setFillColor(color);
	_panels.push_back(res);
	return *res;
}

sf::RectangleShape & GUI::Layer::addOverlayPanel(int w, int h, sf::Color color){
	sf::RectangleShape* res = new sf::RectangleShape(sf::Vector2f(w, h));
	res->setFillColor(color);
	_overlayPanels.push_back(res);
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

