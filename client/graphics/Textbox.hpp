#ifndef GUI_TEXTBOX_HPP
#define GUI_TEXTBOX_HPP


#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "Button.hpp"
#include "GUIConstants.hpp"

namespace GUI {
	class Textbox {
	public:
		Textbox(int x=MARGIN, int y=MARGIN, int w=250, int h=BUTTON_HEIGHT):
					_x(x), _y(y), _w(w), _h(h), _isFocused(false), 
					_selecter(Button<Textbox>(&Textbox::focus, this, "")){

			if (!_font.loadFromFile(BODY_FONT_PATH))
				throw "Could not load font!";
			_text.setFont(_font);
			_text.setString("kewkew"); // TODO delete?
			_text.setCharacterSize(TEXTBOX_TEXT_SIZE);
			_text.setColor(TEXTBOX_TEXT_COLOR);
			_backgroundRect = sf::RectangleShape(sf::Vector2f(_w, _h));
			_backgroundRect.setFillColor(TEXTBOX_BACKGROUND_COLOR);
			_selecter.setBackgroundColor(sf::Color(0x00, 0x00, 0x00, 0x10));
			_selecter.setWidth(_w);
			_selecter.setHeight(_h);
		}
		void renderTo(sf::RenderTarget & dest){
			_backgroundRect.setPosition(_x, _y);
			dest.draw(_backgroundRect);
			_text.setPosition(_x+TEXTBOX_SIDE_PADDING, _y+TEXTBOX_TOP_PADDING);
			dest.draw(_text);
			_selecter.setPosition(_x, _y);
			_selecter.renderTo(dest);
		}

		bool isInBounds (int x, int y) const {
			return ((x >=_x) && (x <= _x+_w) && (y >=_y) && (y <= _y+_h));
		}

		void setPosition(int x, int y){
			_x = x; 
			_y = y;
		}

		void updateText(sf::Event event){
			if (event.text.unicode == '\b' && _text.getString().getSize() != 0){
				sf::String oldText = _text.getString();
				oldText.erase(oldText.getSize()-1, 1);
				_text.setString(oldText);
			}
			else {
				_text.setString(_text.getString() + event.text.unicode);
			}
		}

		void focus(){
			std::cout << "focused!"<< std::endl;
			_isFocused = true;
		}
		void unfocus(){
			_isFocused = false;
		}

		void setText(std::string text) { _text.setString(text); }
		std::string getText() { return std::string(_text.getString()); }
	private:
		sf::Text _text;
		sf::Font _font;
		int _x;
		int _y;
		int _w;
		int _h;
		sf::RectangleShape _backgroundRect;
		bool _isFocused;
		Button<Textbox> _selecter;
	};
}

#endif