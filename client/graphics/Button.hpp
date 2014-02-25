#include <SFML/Graphics.hpp>
#include <string>
#include "Clickable.hpp"
#include <iostream>

using namespace std;

template <typename T> class Button : public Clickable<T> {
public:
	Button(	const typename Clickable<T>::Callback& callback, T* target, 
			std::string text="Button", int x=10, int y=10, int w=100, int h=40):
					Clickable<T>(callback, target),
					_x(x), _y(y), _w(w), _h(h) {

		if (!_font.loadFromFile("fonts/akashi.ttf"))
			throw "Could not load font!";
		_text.setFont(_font);
		_text.setString(text);
		_text.setCharacterSize(20);
		_text.setColor(sf::Color(0xff, 0xff, 0xff, 0xff));
		_w = (int)_text.getLocalBounds().width + 30;
		_backgroundRect = sf::RectangleShape(sf::Vector2f(_w, _h));
		_backgroundRect.setFillColor(sf::Color(0x00, 0xae, 0xef, 0xff));
	}
	bool isInBounds (int x, int y) const {
		return ((x >=_x) && (x <= _x+_w) && (y >=_y) && (y <= _y+_h));
	}
	void renderTo(sf::RenderTarget & dest){
		_backgroundRect.setPosition(_x, _y);
		dest.draw(_backgroundRect);
		_text.setPosition(_x+15, _y+7);
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