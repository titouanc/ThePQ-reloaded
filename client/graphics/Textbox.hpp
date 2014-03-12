#ifndef GUI_TEXTBOX_HPP
#define GUI_TEXTBOX_HPP


#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "Button.hpp"
#include "GUIConstants.hpp"
#include "Widget.hpp"

namespace GUI {
	class Textbox : public Widget {
	public:
		Textbox(std::string id, int x=MARGIN, int y=MARGIN, int w=250, int h=BUTTON_HEIGHT):
					Widget(x, y, w, h, false), _id(id), _isFocused(false), 
					_selecter(Button<Textbox>(&Textbox::focus, this, "")){

			if (!_font.loadFromFile(fontPath(BODY_FONT_PATH)))
				throw "Could not load font!";
			_text.setFont(_font);
			_text.setString(id);
			_text.setCharacterSize(TEXTBOX_TEXT_SIZE);
			_text.setColor(TEXTBOX_TEXT_COLOR);
			_backgroundRect = sf::RectangleShape(sf::Vector2f(_w, _h));
			_backgroundRect.setFillColor(TEXTBOX_BACKGROUND_COLOR);
			_selecter.setBackgroundColor(sf::Color(0x00, 0x00, 0x00, 0x10));
			_selecter.setWidth(_w);
			_selecter.setHeight(_h);
		}
		virtual ~Textbox(){}
		void renderTo(sf::RenderTarget & dest){
			_backgroundRect.setPosition(_x, _y);
			dest.draw(_backgroundRect);
			_text.setPosition(_x+TEXTBOX_SIDE_PADDING, _y+TEXTBOX_TOP_PADDING);
			sf::Text toDraw = getClippedText();
			dest.draw(toDraw);
			_selecter.setPosition(_x, _y);
			_selecter.renderTo(dest);
		}

		bool isInBounds (int x, int y) const {
			return ((x >=_x) && (x <= _x+_w) && (y >=_y) && (y <= _y+_h));
		}

		void updateText(sf::Event event){
			if (event.text.unicode == '\b') {
				if (_text.getString().getSize() != 0){
					sf::String oldText = _text.getString();
					oldText.erase(oldText.getSize()-1, 1);
					_text.setString(oldText);
				}
			}
			else {
				_text.setString(_text.getString() + event.text.unicode);
			}
		}

		sf::Text getClippedText(){
			// returns biggest string at the end of _text that is not bigger
			// than the textbox size.
			sf::Text res = _text;
			sf::String toPrint = res.getString();
			float maxWidth = _w-2*TEXTBOX_SIDE_PADDING;
			if (res.getLocalBounds().width > maxWidth){
				while(res.getLocalBounds().width > maxWidth && toPrint.getSize() > 0){
					toPrint.erase(0, 1);
					res.setString(toPrint);
				}
			}
			return res;
		}

		void focus(){
			_isFocused = true;
		}
		void unfocus(){
			_isFocused = false;
		}

		void setText(std::string text) { _text.setString(text); }
		std::string getText() { return std::string(_text.getString()); }
		void clearText() { 
			sf::String clearedText = _text.getString();
			clearedText.erase(0, clearedText.getSize());
			_text.setString(clearedText); 
		}

		std::string getID() {return _id;}
	private:
		std::string _id;
		sf::Text _text;
		sf::Font _font;
		sf::RectangleShape _backgroundRect;
		bool _isFocused;
		Button<Textbox> _selecter;
	};
}

#endif