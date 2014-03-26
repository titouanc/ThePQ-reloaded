#ifndef TABLE_VIEW_HPP
#define TABLE_VIEW_HPP

#include <SFML/Graphics.hpp>
#include "Layer.hpp"
#include "Widget.hpp"
#include <vector>
#include <map>
#include <string>

namespace GUI {

	class TableCell : public Layer {
	public:
		TableCell(int w=400, int h=100, sf::Color backgroundColor=sf::Color(0xff, 0xff, 0xff, 0xff));
		void setBackgroundColor(sf::Color color);
		void drawTo(sf::RenderTarget & dest);
	private:
		sf::RectangleShape _backgroundRect;
	};


	class TableView : public Widget {
	public:
		TableView(int columns=1, int padding=5, int elementsPerPage=-1);
		virtual ~TableView();
		TableCell & addTableCell(
			int w=400, int h=100, 
			sf::Color backgroundColor=sf::Color::White
		);
		TableCell & addHeader(
			int w=400, int h=100, 
			sf::Color backgroundColor=sf::Color::White
		);
		void append(TableCell* toAppend);
		void renderTo(sf::RenderTarget & dest);
		bool handleClick(int x, int y);
		void setElementsNumberPerPage(int number);
	private:
		int _columnsNbr;
		int _padding;
		int _elementsPerPage;
		int _currentPage;
		TableCell* _header;
		std::vector<TableCell*> _elements;
	};
}


#endif