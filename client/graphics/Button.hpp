#ifndef GUI_BUTTON_HPP
#define GUI_BUTTON_HPP


#include <SFML/Graphics.hpp>
#include <string>
#include "Clickable.hpp"
#include <iostream>
#include "GUIConstants.hpp"
#include "loadPath.hpp"

namespace GUI {
	template <typename T> class Button : public Clickable<T> {
	public:
		Button(	const typename Clickable<T>::Callback& callback, T* target, std::string text="Button", 
				int x=MARGIN, int y=MARGIN, int h=BUTTON_HEIGHT):
									Clickable<T>(callback, target){
			this->_x = x;
			this->_y = y;
			this->_h = h; 
			this->_hidden = false;

			if (!_font.loadFromFile(fontPath(BODY_FONT_PATH)))
				throw "Could not load font!";
			_text.setFont(_font);
			_text.setString(text);
			_text.setCharacterSize(BUTTON_TEXT_SIZE);
			_text.setColor(BUTTON_TEXT_COLOR);
			this->_w = (int)_text.getLocalBounds().width + 2*BUTTON_SIDE_PADDING;
			_backgroundRect = sf::RectangleShape(sf::Vector2f(this->_w, this->_h));
			_backgroundRect.setFillColor(BUTTON_BACKGROUND_COLOR);
		}
		bool isInBounds (int x, int y) const {
			return ((x >=this->_x) && (x <= this->_x+this->_w) && (y >=this->_y) && (y <= this->_y+this->_h));
		}
		void renderTo(sf::RenderTarget & dest){
			_backgroundRect.setPosition(this->_x, this->_y);
			dest.draw(_backgroundRect);
			_text.setPosition(this->_x+BUTTON_SIDE_PADDING, this->_y+BUTTON_TOP_PADDING);
			dest.draw(_text);
		}

		void setWidth(int w){ 
			this->_w = w; 
			_backgroundRect.setSize(sf::Vector2f(this->_w, this->_h));
		}
		void setHeight(int h){ 
			this->_h = h; 
			_backgroundRect.setSize(sf::Vector2f(this->_w, this->_h));
		}

		void setBackgroundColor(sf::Color color){
			_backgroundRect.setFillColor(color);
		}

		void setText(std::string text) { 
			_text.setString(text); 
			this->_w = (int)_text.getLocalBounds().width + 2*BUTTON_SIDE_PADDING;
			_backgroundRect.setSize(sf::Vector2f(this->_w, this->_h));
		} 
	private:
		sf::Text _text;
		sf::Font _font;
		sf::RectangleShape _backgroundRect;
	};
}

#endif