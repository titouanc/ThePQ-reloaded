#ifndef GUI_BUTTON_HPP
#define GUI_BUTTON_HPP


#include <SFML/Graphics.hpp>
#include <string>
#include "Clickable.hpp"
#include <iostream>
#include "GUIConstants.hpp"

namespace GUI {
	template <typename T> class Button : public Clickable<T> {
	public:
		Button(	const typename Clickable<T>::Callback& callback, T* target, std::string text="Button", 
				int x=MARGIN, int y=MARGIN, int w=100, int h=BUTTON_HEIGHT):
									Clickable<T>(callback, target),
									_x(x), _y(y), _w(w), _h(h) {

			if (!_font.loadFromFile(BODY_FONT_PATH))
				throw "Could not load font!";
			_text.setFont(_font);
			_text.setString(text);
			_text.setCharacterSize(BUTTON_TEXT_SIZE);
			_text.setColor(BUTTON_TEXT_COLOR);
			_w = (int)_text.getLocalBounds().width + 2*BUTTON_SIDE_PADDING;
			_backgroundRect = sf::RectangleShape(sf::Vector2f(_w, _h));
			_backgroundRect.setFillColor(BUTTON_BACKGROUND_COLOR);
		}
		bool isInBounds (int x, int y) const {
			return ((x >=_x) && (x <= _x+_w) && (y >=_y) && (y <= _y+_h));
		}
		void renderTo(sf::RenderTarget & dest){
			_backgroundRect.setPosition(_x, _y);
			dest.draw(_backgroundRect);
			_text.setPosition(_x+BUTTON_SIDE_PADDING, _y+BUTTON_TOP_PADDING);
			dest.draw(_text);
		}

		void setPosition(int x, int y){
			_x = x; 
			_y = y;
		}
		void setWidth(int w){ 
			_w = w; 
			_backgroundRect.setSize(sf::Vector2f(_w, _h));
		}
		void setHeight(int h){ 
			_h = h; 
			_backgroundRect.setSize(sf::Vector2f(_w, _h));
		}

		void setBackgroundColor(sf::Color color){
			_backgroundRect.setFillColor(color);
		}

		void setText(std::string text) { _text.setString(text); } // TODO update width
	private:
		sf::Text _text;
		sf::Font _font;
		int _x;
		int _y;
		int _w;
		int _h;
		sf::RectangleShape _backgroundRect;
	};
}

#endif