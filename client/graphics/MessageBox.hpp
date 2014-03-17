#ifndef MESSAGE_BOX_HPP
#define MESSAGE_BOX_HPP

#include "GraphicManager.hpp"
#include <network/ClientConnectionManager.hpp>
#include <string>
#include <iostream>

class MessageBox : public GUI::GraphicManager {
public:
	MessageBox(GUI::MainController &controller, std::string text, std::vector<std::string> options);
	int showBox();

	void displayOptions(int yPos);

	void discard();
	void chooseOption(int option);
private:
	std::string _text;
	std::vector<std::string> _options;
	int _chosenOption;
};



#endif