#ifndef UIBUTTON_HPP
#define UIBUTTON_HPP
#endif

#include <SFML/Graphics.hpp>
#include <string>
#include "UIClickable.hpp"
#include <iostream>
#include "GUIConstants.hpp"

using namespace std;

template <typename T> class UIButton : public UIClickable<T> {
public:
	UIButton(	const typename UIClickable<T>::Callback& callback, T* target, std::string text="Button", 
			int x=GUI::MARGIN, int y=GUI::MARGIN, int w=100, int h=GUI::BUTTON_HEIGHT):
								UIClickable<T>(callback, target),
								_x(x), _y(y), _w(w), _h(h) {

		if (!_font.loadFromFile(GUI::BODY_FONT_PATH))
			throw "Could not load font!";
		_text.setFont(_font);
		_text.setString(text);
		_text.setCharacterSize(GUI::BUTTON_TEXT_SIZE);
		_text.setColor(GUI::BUTTON_TEXT_COLOR);
		_w = (int)_text.getLocalBounds().width + 2*GUI::BUTTON_SIDE_PADDING;
		_backgroundRect = sf::RectangleShape(sf::Vector2f(_w, _h));
		_backgroundRect.setFillColor(GUI::BUTTON_BACKGROUND_COLOR);
	}
	bool isInBounds (int x, int y) const {
		return ((x >=_x) && (x <= _x+_w) && (y >=_y) && (y <= _y+_h));
	}
	void renderTo(sf::RenderTarget & dest){
		_backgroundRect.setPosition(_x, _y);
		dest.draw(_backgroundRect);
		_text.setPosition(_x+GUI::BUTTON_SIDE_PADDING, _y+GUI::BUTTON_TOP_PADDING);
		dest.draw(_text);
	}

	void setText(std::string text) { _text.setString(text); }
private:
	sf::Text _text;
	sf::Font _font;
	int _x;
	int _y;
	int _w;
	int _h;
	sf::RectangleShape _backgroundRect;
};