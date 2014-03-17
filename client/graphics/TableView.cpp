#include "TableView.hpp"

using namespace GUI;

TableCell::TableCell(int w, int h, sf::Color backgroundColor) :
	Layer(backgroundColor), Widget(0, 0, w, h, false), _backgroundRect(sf::Vector2f(w, h))
{
	_backgroundRect.setFillColor(_backgroundColor);
}

void TableCell::renderTo(sf::RenderTarget & dest)
{
	_backgroundRect.setPosition(_x, _y);
	dest.draw(_backgroundRect);
	renderAllAttributesTo(dest);
}

void TableCell::renderAllAttributesTo(sf::RenderTarget &dest)
{
	// rendering clickables
	for(unsigned int i=0; i<_clickables.size(); ++i)
		if (!_clickables[i]->isHidden()){
			_clickables[i]->setPosition(_clickables[i]->x()+_x, _clickables[i]->y()+_y);
			_clickables[i]->renderTo(dest);
			_clickables[i]->setPosition(_clickables[i]->x()-_x, _clickables[i]->y()-_y);
		}
	// rendering textboxes
	std::map<std::string, Textbox*>::iterator it = _textboxes.begin();
	for(; it != _textboxes.end(); it++)
		if (!it->second->isHidden()){
			it->second->setPosition(it->second->x()+_x, it->second->y()+_y);
			it->second->renderTo(dest);
			it->second->setPosition(it->second->x()-_x, it->second->y()-_y);
		}
	// rendering labels
	for(unsigned i=0; i<_labels.size(); ++i){
		if (!_labels[i]->isHidden()){
			int x=_labels[i]->x()+_x, y=_labels[i]->y()+_y;
			_labels[i]->setPosition(x, y);
			_labels[i]->renderTo(dest);
			_labels[i]->setPosition(_labels[i]->x()-_x, _labels[i]->y()-_y);
		}
	}
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
		_elements[i]->renderTo(dest);
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