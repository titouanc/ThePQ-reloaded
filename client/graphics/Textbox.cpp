#include "Textbox.hpp"
#include "Bank.hpp"
#include <toolbox.hpp>

using namespace GUI;

Textbox::Textbox(std::string id):
	Widget(0, 0, 250, BUTTON_HEIGHT, false), _id(id), _isFocused(false), 
	_selecter(Button<Textbox>(&Textbox::focus, this, ""))
{

	_text.setFont(FONTS.get(BODY_FONT_PATH));
	_text.setString(id);
	_text.setCharacterSize(TEXTBOX_TEXT_SIZE);
	_text.setColor(TEXTBOX_TEXT_COLOR);
	_backgroundRect = sf::RectangleShape(sf::Vector2f(_w, _h));
	_backgroundRect.setFillColor(TEXTBOX_BACKGROUND_COLOR);
	_selecter.setBackgroundColor(sf::Color(0x00, 0x00, 0x00, 0x10));
	_selecter.setWidth(_w);
	_selecter.setHeight(_h);
}

Textbox::~Textbox()
{}

void Textbox::renderTo(sf::RenderTarget & dest)
{
	_backgroundRect.setPosition(_drawX, _drawY);
	dest.draw(_backgroundRect);
	_text.setPosition(_drawX+TEXTBOX_SIDE_PADDING, _drawY+TEXTBOX_TOP_PADDING);
	sf::Text toDraw = getClippedText();
	dest.draw(toDraw);
	_selecter.setPosition(_drawX, _drawY);
	_selecter.renderTo(dest);
}

bool Textbox::isInBounds (int x, int y) const 
{
	return ((x >=_drawX) && (x <= _drawX+_w) && (y >=_drawY) && (y <= _drawY+_h));
}

void Textbox::updateText(sf::Event event)
{
	if (event.text.unicode == '\b') {
		if (_text.getString().getSize() != 0){
			sf::String oldText = _text.getString();
			oldText.erase(oldText.getSize()-1, 1);
			_text.setString(oldText);
		}
	}
	else {
		_text.setString(_text.getString() + event.text.unicode);
	}
}

sf::Text Textbox::getClippedText()
{
	// returns biggest string at the end of _text that is not bigger
	// than the textbox size.
	sf::Text res = _text;
	sf::String toPrint = res.getString();
	float maxWidth = _w-2*TEXTBOX_SIDE_PADDING;
	if (res.getLocalBounds().width > maxWidth){
		while(res.getLocalBounds().width > maxWidth && toPrint.getSize() > 0){
			toPrint.erase(0, 1);
			res.setString(toPrint);
		}
	}
	return res;
}

void Textbox::focus()
{
	_isFocused = true;
}
void Textbox::unfocus()
{
	_isFocused = false;
}

void Textbox::setText(std::string text) 
{
	_text.setString(text);
}

std::string Textbox::getText() 
{
	return strip(std::string(_text.getString()));
}

void Textbox::clearText() 
{ 
	sf::String clearedText = _text.getString();
	clearedText.erase(0, clearedText.getSize());
	_text.setString(clearedText); 
}

std::string Textbox::getID() 
{
	return _id;
}
