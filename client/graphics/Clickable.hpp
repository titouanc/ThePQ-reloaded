#ifndef GUI_CLICKABLE_HPP
#define GUI_CLICKABLE_HPP 

#include <functional>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace GUI{
	class ClickableInterface {
	public:
		virtual void triggerAction() = 0;
		virtual bool isInBounds(int x, int y) const = 0;
		virtual ~ClickableInterface() {};
		virtual void renderTo(sf::RenderTarget& dest) = 0;
	};

	template <typename T> 
	class Clickable : public ClickableInterface {
	public:
		typedef std::function<void(T*)> Callback;
		Clickable(const Callback& callback, T* target) : _callback(callback), _target(target) {}
		~Clickable(){}
		void triggerAction(){ _callback(_target); }
		virtual bool isInBounds(int x, int y) const = 0;
		virtual void renderTo(sf::RenderTarget& dest) = 0;
	private:
		Callback _callback;
		T* _target;
	};
}

#endif