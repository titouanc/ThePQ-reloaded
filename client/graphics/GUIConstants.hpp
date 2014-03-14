#ifndef __GUICONSTANTS_HPP
#define __GUICONSTANTS_HPP

#include <SFML/Graphics/Color.hpp>
#include <string>


namespace GUI{
	// General
	static const int MARGIN = 10;
	static const int WINDOW_WIDTH = 1280;
	static const int WINDOW_HEIGHT = 720;

	// Fonts
	static const std::string HEADER_FONT_PATH = "akashi.ttf";
	static const std::string BODY_FONT_PATH = "helvetica-neue-light.ttf";
	static const sf::Color BODY_TEXT_COLOR = sf::Color(0x33, 0x33, 0x33, 0xff);	
	static const int BODY_TEXT_SIZE = 20;	
	static const sf::Color BLUE_TEXT_COLOR = sf::Color(0x00, 0xae, 0xef, 0xff);
	static const sf::Color GREEN_TEXT_COLOR = sf::Color(0x8b, 0xbb, 0x25, 0xff);
	static const sf::Color RED_TEXT_COLOR = sf::Color(0xff,0x00,0x00,0xff);

	// Buttons
	static const int BUTTON_HEIGHT = 40;
	static const int BUTTON_SIDE_PADDING = 15;
	static const int BUTTON_TOP_PADDING = 7;
	static const int BUTTON_TEXT_SIZE = 20;
	static const sf::Color BUTTON_BACKGROUND_COLOR = sf::Color(0x00, 0xae, 0xef, 0xff);
	static const sf::Color DARK_BUTTON_BACKGROUND_COLOR = sf::Color(0x33, 0x33, 0x33, 0xff);
	static const sf::Color LIGHT_BUTTON_BACKGROUND_COLOR = sf::Color(0xcc, 0xcc, 0xcc, 0xff);
	static const sf::Color GREEN_BUTTON_BACKGROUND_COLOR = sf::Color(0x8b, 0xbb, 0x25, 0xff);
	static const sf::Color RED_BUTTON_BACKGROUND_COLOR = sf::Color(0xb5,0x00,0x00,0xff);
	static const sf::Color BUTTON_TEXT_COLOR = sf::Color(0xff, 0xff, 0xff, 0xff);
	static const sf::Color LIGHT_BUTTON_TEXT_COLOR = sf::Color(0x33, 0x33, 0x33, 0xff);

	static const sf::Color TEXTBOX_BACKGROUND_COLOR = sf::Color(0xff, 0xff, 0xff, 0xff);
	static const int TEXTBOX_SIDE_PADDING = 5;
	static const int TEXTBOX_TOP_PADDING = 7;
	static const int TEXTBOX_TEXT_SIZE = 20;
	static const sf::Color TEXTBOX_TEXT_COLOR = sf::Color(0x33, 0x33, 0x33, 0xff);

}

#endif