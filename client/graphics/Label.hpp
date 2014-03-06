#ifndef LABEL_HPP
#define LABEL_HPP


#include <SFML/Graphics.hpp>
#include <string>
#include "GUIConstants.hpp"
#include "Widget.hpp"

namespace GUI {
	class Label : public Widget{
	public:
		Label(std::string text, sf::Color color=BODY_TEXT_COLOR){
			if (!_font.loadFromFile(fontPath(BODY_FONT_PATH)))
				throw "Could not load font!";
			_text.setFont(_font);
			_text.setString(text);
			_text.setCharacterSize(BODY_TEXT_SIZE);
			_text.setColor(color);
		}

		void renderTo(sf::RenderTarget & dest){
			_text.setPosition(_x, _y);
			dest.draw(_text);
		}

		void setText(std::string text) { 
			_text.setString(text); 
			_w = _text.getLocalBounds().width;
			_h = _text.getLocalBounds().height;
		}
		void setColor(sf::Color color) { _text.setColor(color); }
private:
		sf::Text _text;
		sf::Font _font;
	};
}


#endif