#ifndef MESSAGE_BOX_HPP
#define MESSAGE_BOX_HPP

#include "GraphicManager.hpp"
#include <network/ClientConnectionManager.hpp>
#include <string>
#include <iostream>

class MessageBox : public GUI::GraphicManager {
public:
	MessageBox(GUI::MainController &controller, std::string text, std::vector<std::string> options) : 
		GraphicManager(controller), _text(text), _options(options){
		_canvas.setBackgroundColor(sf::Color(0x00, 0x00, 0x00, 0x99));
		displayCanvas();
	}
	int showBox(){
		int hCenter = window().getSize().x/2;
		int vCenter = window().getSize().y/2;
		GUI::Label & textLabel = _canvas.addLabel(_text);
		sf::RectangleShape & box = _canvas.addPanel(textLabel.getWidth()+100, textLabel.getHeight()+100, sf::Color(0xff, 0xff, 0xff, 0xff));

		textLabel.setPosition(hCenter-textLabel.getWidth()/2, vCenter-textLabel.getHeight()/2-5);
		box.setPosition(hCenter-box.getSize().x/2, vCenter-box.getSize().y/2);

		displayOptions(vCenter+box.getSize().y/2+GUI::MARGIN);

		_canvas.renderAllAttributesTo(window());
		window().display();
		run();
		return _chosenOption;
	}

	void displayOptions(int yPos){
		int totalLength = -GUI::MARGIN; // this will be used to center all the buttons
		std::vector<GUI::Button<MessageBox>*> buttons;

		for (int i=0; i<_options.size(); ++i){
			GUI::Button<MessageBox> &button = _canvas.addButton<MessageBox, int>(&MessageBox::chooseOption, i, this, _options[i]);	
			totalLength += button.getWidth()+GUI::MARGIN;
			buttons.push_back(&button);
		}

		int hCenter = window().getSize().x/2;
		int xBegin = hCenter-totalLength/2; // the x coordinate where we have to start to center all buttons
		int currentX = xBegin; // will be used to set the X position of the buttons

		for (GUI::Button<MessageBox>* button : buttons){
			button->setPosition(currentX, yPos);
			currentX += button->getWidth()+GUI::MARGIN;
		}

	}

	void discard(){
		deleteCanvas();
		stop();
	}
	void chooseOption(int option){
		_chosenOption = option;
		discard();
	}
private:
	std::string _text;
	std::vector<std::string> _options;
	int _chosenOption;
};



#endif