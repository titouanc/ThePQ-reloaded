#include "TableView.hpp"

using namespace GUI;

TableCell::TableCell(int w, int h, sf::Color backgroundColor) :
	Layer(backgroundColor), _backgroundRect(sf::Vector2f(w, h))
{
	setWidth(w);
	setHeight(h);
	_backgroundRect.setFillColor(_backgroundColor);
}

void TableCell::setBackgroundColor(sf::Color color) 
{ 
	_backgroundRect.setFillColor(color); 
}

void TableCell::drawTo(sf::RenderTarget & dest){
	_backgroundRect.setPosition(_drawX, _drawY);
	dest.draw(_backgroundRect);
	renderTo(dest, false);
}

TableView::TableView(int columns, int padding) : 
	Widget(MARGIN, MARGIN, 400, 600, false), _columnsNbr(columns), _padding(padding) 
{}

TableView::~TableView()
{
	for (unsigned int i = 0; i < _elements.size(); ++i) 
		delete _elements[i];
	_elements.clear();
}

TableCell & TableView::addTableCell(int w, int h, sf::Color backgroundColor)
{
	TableCell *res = new TableCell(w, h, backgroundColor);
	append(res);
	return *res;
}

void TableView::append(TableCell* toAppend)
{ 
	_elements.push_back(toAppend); 
}

void TableView::renderTo(sf::RenderTarget & dest)
{
	int x, y;
	for (unsigned int i=0; i<_elements.size(); ++i){
		int col=i%_columnsNbr, row=i/_columnsNbr;
		if (col == 0)
			x = _drawX;
		else
			x += _elements[col-1]->getWidth() + _padding;
		y = _drawY + row*(_elements[0]->getHeight()) + row*_padding;
		_elements[i]->setPosition(x, y);
		_elements[i]->drawTo(dest);
	}
}

bool TableView::handleClick(int x, int y)
{
	bool res = false;
	for (unsigned int i=0; i< _elements.size(); ++i){
		res = _elements[i]->handleClick(x, y);
		if (res) return res;
	}
	return false;
}