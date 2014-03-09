#ifndef TABLE_VIEW_HPP
#define TABLE_VIEW_HPP

#include <SFML/Graphics.hpp>
#include "Layer.hpp"
#include "Widget.hpp"
#include <vector>
#include <map>
#include <string>

namespace GUI {

	class TableCell : public Layer, public Widget {
	public:
		TableCell(int w=400, int h=100, sf::Color backgroundColor=sf::Color(0xff, 0xff, 0xff, 0xff)) :
					Layer(backgroundColor), Widget(0, 0, w, h, false), _backgroundRect(sf::Vector2f(w, h))
		{
			_backgroundRect.setFillColor(_backgroundColor);
		}
		void renderTo(sf::RenderTarget & dest){
			_backgroundRect.setPosition(_x, _y);
			dest.draw(_backgroundRect);
			renderAllAttributesTo(dest);
		}
		void renderAllAttributesTo(sf::RenderTarget &dest){
			// this method allows users of this class to use relative positions. 
			// We can set a label position to (10, 10) and it will always be 
			// at (10, 10) relatively to its cell.

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

		void setBackgroundColor(sf::Color color) { _backgroundRect.setFillColor(color); }
		
	private:
		sf::RectangleShape _backgroundRect;
	};


	class TableView : public Widget {
	public:
		TableView(int columns=1, int padding=5) : 
					Widget(MARGIN, MARGIN, 400, 600, false), _columnsNbr(columns), 
					_padding(padding) {
		}
		~TableView(){
			for (unsigned int i = 0; i < _elements.size(); ++i) 
				delete _elements[i];
			_elements.clear();
		}
		TableCell & addTableCell(int w=400, int h=100, sf::Color backgroundColor=sf::Color(0xff, 0xff, 0xff, 0xff)){
			TableCell *res = new TableCell(w, h, backgroundColor);
			append(res);
			return *res;
		}
		void append(TableCell* toAppend){ 
			_elements.push_back(toAppend); 
		}
		void renderTo(sf::RenderTarget & dest){
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

		void handleClick(int x, int y){
			if (_elements.size() > 0){
				int clickedCol = (x-_x)/(_elements[0]->getHeight()+_padding);
				// we have to check if the click is not on a padding zone.
				int relativePosX = (x-_x)%(_elements[0]->getHeight()+_padding);
				if (relativePosX <= _elements[0]->getHeight()){
					int clickedRow = (y-_y)/(_elements[0]->getWidth()+_padding);

					int relativePosY = (y-_y)%(_elements[0]->getWidth()+_padding);
					if (relativePosY <= _elements[0]->getWidth()){
						unsigned int index = clickedRow * _columnsNbr + clickedCol;
						if (index < _elements.size())
							_elements[index]->handleClick(relativePosX, relativePosY);
					}
				}

			}
		}
	private:
		int _columnsNbr;
		int _padding;
		std::vector<TableCell*> _elements;
	};
}


#endif