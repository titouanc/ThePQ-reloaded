#ifndef GUI_CLICKABLE_HPP
#define GUI_CLICKABLE_HPP 

#include <functional>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Widget.hpp"

namespace GUI{
	class ClickableInterface : public Widget{
	public:
		virtual void triggerAction() = 0;
		virtual bool isInBounds(int x, int y) const = 0;
		virtual ~ClickableInterface() {};
		virtual void renderTo(sf::RenderTarget& dest) = 0;
	};

	template <typename T, typename P=int> 
	class Clickable : public ClickableInterface {
	public:
		typedef std::function<void(T*)> Callback;
		typedef std::function<void(T*, P)> CallbackWithParam;
		Clickable(
			const Callback& callback, 
			T* target
		) : 
			_callback(callback), 
			_target(target), 
			_hasParam(false) 
		{}

		Clickable(
			const CallbackWithParam& callback,
			T* target, 
			P param
		) : 
			_callbackWithParam(callback),
			_target(target), 
			_param(param), 
			_hasParam(true)
		{}
		
		virtual ~Clickable(){}
		
		void triggerAction(){ 
			if (_hasParam)
				_callbackWithParam(_target, _param);
			else
				_callback(_target); 
		}
		virtual bool isInBounds(int x, int y) const = 0;
		virtual void renderTo(sf::RenderTarget& dest) = 0;
	private:
		Callback _callback;
		CallbackWithParam _callbackWithParam;
		T* _target;
		P _param;
		bool _hasParam;
	};
}

#endif