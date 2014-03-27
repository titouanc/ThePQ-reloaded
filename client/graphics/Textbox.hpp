#ifndef GUI_TEXTBOX_HPP
#define GUI_TEXTBOX_HPP


#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "Button.hpp"
#include "GUIConstants.hpp"
#include "Widget.hpp"

namespace GUI {
	class Textbox : public Widget {
	public:
		Textbox(std::string id);
		virtual ~Textbox();
		void renderTo(sf::RenderTarget & dest);

		bool isInBounds (int x, int y) const;

		void updateText(sf::Event event);

		sf::Text getClippedText();

		void focus();
		void unfocus();

		void setText(std::string text);
		std::string getText();
		void clearText();

		std::string getID();
	private:
		std::string _id;
		sf::Text _text;
		sf::RectangleShape _backgroundRect;
		bool _isFocused;
		Button<Textbox> _selecter;
	};
}

#endif