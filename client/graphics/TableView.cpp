#include "TableView.hpp"
#include <algorithm>

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

TableView::TableView(int columns, int padding, int elementsPerPage) : 
	Widget(MARGIN, MARGIN, 400, 600, false), _columnsNbr(columns), _padding(padding),
	_elementsPerPage(elementsPerPage), _currentPage(0), _header(NULL), _prevButton(NULL),
	_nextButton(NULL)
{}

TableView::~TableView()
{
	for (unsigned int i = 0; i < _elements.size(); ++i) 
		delete _elements[i];
	_elements.clear();
	if (_header) delete _header;
}

TableCell & TableView::addTableCell(int w, int h, sf::Color backgroundColor)
{
	TableCell *res = new TableCell(w, h, backgroundColor);
	append(res);
	return *res;
}

TableCell & TableView::addHeader(int w, int h, sf::Color backgroundColor)
{
	_header = new TableCell(w, h, backgroundColor);
	return *_header;
}


void TableView::append(TableCell* toAppend)
{ 
	_elements.push_back(toAppend); 
}

void TableView::renderTo(sf::RenderTarget & dest)
{
	int x, y;
	int start = _elementsPerPage*_currentPage;
	int end = std::min(_elementsPerPage*(_currentPage+1), int(_elements.size()));
	if (_elementsPerPage == -1) {
		start = 0;
		end = _elements.size();
	}
	// Drawing header
	if (_header){
		_header->setPosition(_drawX, _drawY);
		_header->drawTo(dest);
	}
	// Drawing other cells
	for (int i=start; i<end; ++i){
		int col=(i-start)%_columnsNbr, row=(i-start)/_columnsNbr;
		if (col == 0)
			x = _drawX;
		else
			x += _elements[col-1]->getWidth() + _padding;
		y = _drawY + row*(_elements[0]->getHeight()) + row*_padding;
		if (_header) y = y + (_elements[0]->getHeight()) + _padding;
		_elements[i]->setPosition(x, y);
		_elements[i]->drawTo(dest);
	}
	y += (_elements[0]->getHeight()) + _padding;
	x = _drawX;
	// Drawing pager

	if (_prevButton) delete _prevButton;
	if (_nextButton) delete _nextButton;
	if (_elementsPerPage < int(_elements.size())){
		_prevButton = new Button<TableView>(
			&TableView::goToPreviousPage, this, "<");
		_prevButton->setPosition(x, y);
		_nextButton = new Button<TableView>(
			&TableView::goToNextPage, this, ">");
		_nextButton->setPosition(x+_prevButton->getWidth()+MARGIN, y);
		_prevButton->renderTo(dest);
		_nextButton->renderTo(dest);
	}
}

bool TableView::handleClick(int x, int y)
{
	bool res = false;
	for (unsigned int i=0; i< _elements.size(); ++i){
		res = _elements[i]->handleClick(x, y);
		if (res) return res;
	}
	if (_prevButton && _prevButton->isInBounds(x, y))
		_prevButton->triggerAction();
	if (_nextButton && _nextButton->isInBounds(x, y))
		_nextButton->triggerAction();
	return false;
}

void TableView::setElementsNumberPerPage(int number){
	_elementsPerPage = number;
}

void TableView::goToNextPage(){
	if (_currentPage < _elements.size()/_elementsPerPage)
		_currentPage++;
}

void TableView::goToPreviousPage(){
	if (_currentPage > 0) _currentPage--;
}