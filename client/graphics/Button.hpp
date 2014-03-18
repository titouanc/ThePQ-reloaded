#ifndef GUI_BUTTON_HPP
#define GUI_BUTTON_HPP


#include <SFML/Graphics.hpp>
#include <string>
#include "Clickable.hpp"
#include <iostream>
#include "GUIConstants.hpp"
#include "loadPath.hpp"

namespace GUI {
	template <typename T, typename P=int> class Button : public Clickable<T, P> {
	public:
		Button(
			const typename Clickable<T>::Callback& callback, 
			T* target, 
			std::string text="Button",
			sf::Color color=BUTTON_BACKGROUND_COLOR
		):
			Clickable<T>(callback, target)
		{
			this->_h = BUTTON_HEIGHT; 
			this->_hidden = false;

			if (!_font.loadFromFile(fontPath(BODY_FONT_PATH)))
				throw "Could not load font!";
			_text.setFont(_font);
			if (text.length() != 0)
				_text.setString(text);
			else
				_text.setString(" ");
			_text.setCharacterSize(BUTTON_TEXT_SIZE);
			_text.setColor(BUTTON_TEXT_COLOR);
			this->_w = (int)_text.getLocalBounds().width + 2*BUTTON_SIDE_PADDING;
			_backgroundRect = sf::RectangleShape(sf::Vector2f(this->_w, this->_h));
			_backgroundRect.setFillColor(color);
			_highlightRect = _backgroundRect;
			_highlightRect.setFillColor(sf::Color(0xff, 0xff, 0xff, 0x00));
		}

		Button(
			const typename Clickable<T, P>::CallbackWithParam& callback, 
			P param,
			T* target, 
			std::string text="Button",
			sf::Color color=BUTTON_BACKGROUND_COLOR
		):
			Clickable<T, P>(callback, target, param)
		{
			this->_h = BUTTON_HEIGHT; 
			this->_hidden = false;

			if (!_font.loadFromFile(fontPath(BODY_FONT_PATH)))
				throw "Could not load font!";
			_text.setFont(_font);
			if (text.length() != 0)
				_text.setString(text);
			else
				_text.setString(" ");
			_text.setCharacterSize(BUTTON_TEXT_SIZE);
			_text.setColor(BUTTON_TEXT_COLOR);
			this->_w = (int)_text.getLocalBounds().width + 2*BUTTON_SIDE_PADDING;
			_backgroundRect = sf::RectangleShape(sf::Vector2f(this->_w, this->_h));
			_backgroundRect.setFillColor(color);
			_highlightRect = _backgroundRect;
			_highlightRect.setFillColor(sf::Color(0xff, 0xff, 0xff, 0x00));
		}

		bool isInBounds (int x, int y) const {
			return ((x >=this->_x) && (x <= this->_x+this->_w) && (y >=this->_y) && (y <= this->_y+this->_h));
		}

		void renderTo(sf::RenderTarget & dest){
			_backgroundRect.setPosition(this->_x, this->_y);
			dest.draw(_backgroundRect);
			_text.setPosition(this->_x+BUTTON_SIDE_PADDING, this->_y+BUTTON_TOP_PADDING);
			dest.draw(_text);
			_highlightRect.setPosition(this->_x, this->_y);
			dest.draw(_highlightRect);
		}

		void setWidth(int w){ 
			this->_w = w; 
			_backgroundRect.setSize(sf::Vector2f(this->_w, this->_h));
			_highlightRect.setSize(sf::Vector2f(this->_w, this->_h));
		}
		
		void setHeight(int h){ 
			this->_h = h; 
			_backgroundRect.setSize(sf::Vector2f(this->_w, this->_h));
			_highlightRect.setSize(sf::Vector2f(this->_w, this->_h));
		}

		void setBackgroundColor(sf::Color color){
			_backgroundRect.setFillColor(color);
		}

		void setTextColor(sf::Color color){
			_text.setColor(color);
		}

		void setText(std::string text) { 
			if (text.length() != 0)
				_text.setString(text);
			else
				_text.setString(" ");
			this->_w = (int)_text.getLocalBounds().width + 2*BUTTON_SIDE_PADDING;
			_backgroundRect.setSize(sf::Vector2f(this->_w, this->_h));
			_highlightRect.setSize(sf::Vector2f(this->_w, this->_h));
		} 

		void onMouseMoved(int x, int y){
			if (isInBounds(x, y))
				_highlightRect.setFillColor(sf::Color(0xff, 0xff, 0xff, 0x44));
			else
				_highlightRect.setFillColor(sf::Color(0xff, 0xff, 0xff, 0x00));

		}
	private:
		sf::Text _text;
		sf::Font _font;
		sf::RectangleShape _backgroundRect;
		sf::RectangleShape _highlightRect;
	};
}

#endif