#include "Label.hpp"
#include "loadPath.hpp"
#include "Bank.hpp"

using namespace GUI;

Label::Label(std::string text, sf::Color color)
{
	_text.setFont(FONTS.get(BODY_FONT_PATH));
	if (text.length() != 0)
		_text.setString(text);
	else
		_text.setString(" ");
	_text.setCharacterSize(BODY_TEXT_SIZE);
	_text.setColor(color);
	_w = _text.getLocalBounds().width;
	_h = _text.getLocalBounds().height;
}

Label::Label(int number, sf::Color color)
{
	_text.setFont(FONTS.get(BODY_FONT_PATH));
	char casted[512];
	sprintf(casted, "%d", number);
	_text.setString(casted);
	_text.setCharacterSize(BODY_TEXT_SIZE);
	_text.setColor(color);
	_w = _text.getLocalBounds().width;
	_h = _text.getLocalBounds().height;
}

Label::~Label()
{}

void Label::renderTo(sf::RenderTarget & dest)
{
	_text.setPosition(_drawX, _drawY);
	dest.draw(_text);
}

void Label::setText(std::string text)
{ 
	if (text.length() != 0)
		_text.setString(text);
	else
		_text.setString(" "); /* sf::String bug on OSX */
	_w = _text.getLocalBounds().width;
	_h = _text.getLocalBounds().height;
}

void Label::setFontSize(int fontSize)
{
	_text.setCharacterSize(fontSize);
}

void Label::setFont(std::string path)
{
	_text.setFont(FONTS.get(path));
}

void Label::setColor(sf::Color color)
{ 
	_text.setColor(color); 
}