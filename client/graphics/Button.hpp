#include <SFML/Graphics.hpp>
#include "Clickable.hpp"

template <typename T> class Button : public Clickable<T> {
public:
	Button(const typename Clickable<T>::Callback& callback, T* target, int x=10, int y=10, int w=100, int h=40):
					Clickable<T>(callback, target),
					_x(x), _y(y), _w(w), _h(h),
					_backgroundRect(sf::Vector2f(_w, _h)) {
		_backgroundRect.setFillColor(sf::Color(0x00, 0xae, 0xef, 0xff));
	}
	void renderTo(sf::RenderTarget & dest){
		_backgroundRect.setPosition(_x, _y);
		dest.draw(_backgroundRect);
	}
private:
	int _x;
	int _y;
	int _w;
	int _h;
	sf::RectangleShape _backgroundRect;
};