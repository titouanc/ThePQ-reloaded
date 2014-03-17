#ifndef LABEL_HPP
#define LABEL_HPP


#include <SFML/Graphics.hpp>
#include <string>
#include "GUIConstants.hpp"
#include "Widget.hpp"

namespace GUI {
	class Label : public Widget {
	public:
		Label(std::string text, sf::Color color=BODY_TEXT_COLOR);
		Label(int number, sf::Color color=BODY_TEXT_COLOR);
		virtual ~Label();

		void renderTo(sf::RenderTarget & dest);

		void setText(std::string text);

		void setFontSize(int fontSize);
		void setFont(std::string path);
		void setColor(sf::Color color);
	private:
		sf::Text _text;
		sf::Font _font;
	};
}


#endif