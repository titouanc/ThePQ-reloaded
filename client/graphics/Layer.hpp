#ifndef GUI_LAYER_HPP
#define GUI_LAYER_HPP 

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <string>
#include "Button.hpp"
#include "Textbox.hpp"
#include "GUIConstants.hpp"

namespace GUI {
	class Layer {
	public:
		Layer(sf::Color backgroundColor=sf::Color(0xff, 0xff, 0xff, 0xff)): 
				_active(false), _backgroundColor(backgroundColor), _focusedTextbox(NULL){}
		~Layer();

		bool isActive() 	{ return _active; }
		void activate() 	{ _active = true; }
		void deactivate() 	{ _active = false; }

		void handleClick(int x, int y);
		void renderTo(sf::RenderTarget & dest);

		template <typename T> 
		GUI::Button<T>* addButton(	const typename GUI::Clickable<T>::Callback& callback, 
								T* target, std::string text="Button");
		GUI::Textbox* addTextbox(std::string id);

		GUI::Textbox* textboxWithID(std::string id);
		void unfocusAllTextboxes();
		void handleTextEntered(sf::Event event);
	private:
		bool _active;
		sf::Color _backgroundColor;
		std::vector<GUI::ClickableInterface*> _clickables;
		std::map<std::string, GUI::Textbox*> _textboxes;
		GUI::Textbox* _focusedTextbox;
	};
}

template <typename T>
GUI::Button<T> * GUI::Layer::addButton(const typename GUI::Clickable<T>::Callback& callback, 
									T* target, std::string text) {
	GUI::Button<T> * toAdd = new GUI::Button<T>(callback, target, text);
	_clickables.push_back((GUI::ClickableInterface*)toAdd);
	return toAdd;
}


#endif