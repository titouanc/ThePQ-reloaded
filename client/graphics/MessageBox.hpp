#ifndef MESSAGE_BOX_HPP
#define MESSAGE_BOX_HPP

#include "GraphicManager.hpp"
#include <network/ClientConnectionManager.hpp>
#include <string>
#include <iostream>

class MessageBox : public GUI::GraphicManager {
public:
	MessageBox(GUI::MainController &controller, std::string text) : 
		GraphicManager(controller), _text(text){
		_canvas.setBackgroundColor(sf::Color(0x00, 0x00, 0x00, 0x99));
		displayCanvas();
	}
	void drawBox(){
		int hCenter = window().getSize().x/2;
		int vCenter = window().getSize().y/2;
		GUI::Label & textLabel = _canvas.addLabel(_text);
		sf::RectangleShape & box = _canvas.addPanel(textLabel.getWidth()+100, textLabel.getHeight()+100, sf::Color(0xff, 0xff, 0xff, 0xff));

		textLabel.setPosition(hCenter-textLabel.getWidth()/2, vCenter-box.getSize().y/2+30);
		box.setPosition(hCenter-box.getSize().x/2, vCenter-box.getSize().y/2);

		GUI::Button<MessageBox>  &okButton = _canvas.addButton<MessageBox>(&MessageBox::discard, this, "OK");
		okButton.setPosition(hCenter-okButton.getWidth()/2, vCenter+box.getSize().y/2-30);
		
		_canvas.renderAllAttributesTo(window());
		window().display();
		run();
	}
	void discard(){
		deleteCanvas();
		stop();
	}
private:
	std::string _text;
};



#endif