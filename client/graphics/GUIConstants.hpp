#ifndef __GUICONSTANTS_HPP
#define __GUICONSTANTS_HPP

#include <SFML/Graphics/Color.hpp>
#include <string>


namespace GUI{
	// General
	static const int MARGIN = 10;

	// Fonts
	static const std::string HEADER_FONT_PATH = "fonts/akashi.ttf";
	static const std::string BODY_FONT_PATH = "fonts/helvetica-neue-light.ttf";

	// Buttons
	static const int BUTTON_HEIGHT = 40;
	static const int BUTTON_SIDE_PADDING = 15;
	static const int BUTTON_TOP_PADDING = 7;
	static const int BUTTON_TEXT_SIZE = 20;
	static const sf::Color BUTTON_BACKGROUND_COLOR = sf::Color(0x00, 0xae, 0xef, 0xff);
	static const sf::Color BUTTON_TEXT_COLOR = sf::Color(0xff, 0xff, 0xff, 0xff);

}

#endif