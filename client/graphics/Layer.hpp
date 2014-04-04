#ifndef GUI_LAYER_HPP
#define GUI_LAYER_HPP 

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <string>
#include "Button.hpp"
#include "Textbox.hpp"
#include "Label.hpp"
#include "GUIConstants.hpp"

namespace GUI {

	class TableCell;
	class TableView;

	class Layer : public Widget {
	public:
		Layer(sf::Color backgroundColor=sf::Color(0xff, 0xff, 0xff, 0x00)):
				Widget(0, 0, 1280, 720), 
				_backgroundColor(backgroundColor), 
				_backgroundImage(NULL), _focusedTextbox(NULL){}
		virtual ~Layer();

		void clear(bool clearBackground = false);

		bool handleClick(int x, int y);
		bool handleRightClick(int x, int y);
		virtual void renderTo(sf::RenderTarget & dest, bool background=true);
		virtual void renderAllAttributesTo(sf::RenderTarget &dest);

		template <typename T> 
		GUI::Button<T> & addButton(
			const typename GUI::Clickable<T>::Callback& callback, 
			T* target, 
			std::string text="Button", 
			sf::Color backgroundColor = BUTTON_BACKGROUND_COLOR
		);
		
		template <typename T, typename P>
		GUI::Button<T, P> & addButton(
			const typename GUI::Clickable<T, P>::CallbackWithParam& callback, 
			P param, 
			T* target, 
			std::string text="Button", 
			sf::Color backgroundColor = BUTTON_BACKGROUND_COLOR
		);

		void setBackgroundImage(std::string path);
		void setPosition(int x, int y);
		void deleteBackgroundImage();

		GUI::Textbox & addTextbox(std::string id);
		GUI::Label & addLabel(std::string text, sf::Color color=BODY_TEXT_COLOR);
		GUI::Label & addLabel(double val, sf::Color color=BODY_TEXT_COLOR);
		GUI::TableView & addTableView(int columns=1, int padding=5);
		sf::RectangleShape & addPanel(int w, int h, sf::Color color=sf::Color(0xff, 0xff, 0xff, 0xff));
		sf::RectangleShape & addOverlayPanel(int w, int h, sf::Color color=sf::Color(0xff, 0xff, 0xff, 0xff));

		int showMessage(std::string text, std::vector<std::string> options={"OK"});
		void discardMessage();

		GUI::Textbox & textboxWithID(std::string id);
		void unfocusAllTextboxes();
		void handleTextEntered(sf::Event event);
		bool handleMouseMoved(sf::Event event);

		void setBackgroundColor(sf::Color color) {_backgroundColor = color;}
		sf::Color getBackgroundColor() { return _backgroundColor; }

	protected:
		sf::Color _backgroundColor;
		sf::Sprite *_backgroundImage;
		std::vector<GUI::ClickableInterface*> _clickables;
		std::map<std::string, GUI::Textbox*> _textboxes;
		std::vector<GUI::Label*> _labels;
		std::vector<GUI::TableView*> _tableViews;
		std::vector<sf::RectangleShape*> _panels;
		std::vector<sf::RectangleShape*> _overlayPanels;
		GUI::Textbox* _focusedTextbox;
	};
}

template <typename T>
GUI::Button<T> & GUI::Layer::addButton(
	const typename GUI::Clickable<T>::Callback& callback, 
	T* target, 
	std::string text, 
	sf::Color backgroundColor
){
	GUI::Button<T> * toAdd = new GUI::Button<T>(callback, target, text, backgroundColor);
	toAdd->updateDrawPosition(this->_x, this->_y);
	_clickables.push_back((GUI::ClickableInterface*)toAdd);
	return *toAdd;
}

template <typename T, typename P>
GUI::Button<T, P> & GUI::Layer::addButton(
	const typename GUI::Clickable<T, P>::CallbackWithParam& callback, 
	P param, 
	T* target, 
	std::string text, 
	sf::Color backgroundColor
){
	GUI::Button<T, P> * toAdd = new GUI::Button<T, P>(callback, param, target, text, backgroundColor);
	toAdd->updateDrawPosition(this->_x, this->_y);
	_clickables.push_back((GUI::ClickableInterface*)toAdd);
	return *toAdd;
}


#endif