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
			x = _x;
		else
			x += _elements[col-1]->getWidth() + _padding;
		y = _y + row*(_elements[0]->getHeight()) + row*_padding;
		_elements[i]->setPosition(x, y);
		_elements[i]->renderTo(dest, false);
	}
}

bool TableView::handleClick(int x, int y)
{
	if (_elements.size() > 0){
		int clickedCol = (x-_x)/(_elements[0]->getWidth()+_padding);
		// we have to check if the click is not on a padding zone.
		int relativePosX = (x-_x)%(_elements[0]->getWidth()+_padding);
		if (relativePosX <= _elements[0]->getWidth()){
			int clickedRow = (y-_y)/(_elements[0]->getHeight()+_padding);
			int relativePosY = (y-_y)%(_elements[0]->getHeight()+_padding);
			if (relativePosY <= _elements[0]->getHeight()){
				unsigned int index = clickedRow * _columnsNbr + clickedCol;
				if (index < _elements.size())
					return _elements[index]->handleClick(relativePosX, relativePosY);
			}
		}

	}
	return false;
}