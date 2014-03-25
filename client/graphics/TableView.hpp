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
		void renderTo(sf::RenderTarget & dest);
		/*! this method allows users of this class to use relative positions. 
		 *  We can set a label position to (10, 10) and it will always be 
		 *  at (10, 10) relatively to its cell.
		 */
		void renderAllAttributesTo(sf::RenderTarget &dest);

		void setBackgroundColor(sf::Color color);
		
	private:
		sf::RectangleShape _backgroundRect;
	};


	class TableView : public Widget {
	public:
		TableView(int columns=1, int padding=5);
		virtual ~TableView();
		TableCell & addTableCell(
			int w=400, int h=100, 
			sf::Color backgroundColor=sf::Color::White
		);
		void append(TableCell* toAppend);
		void renderTo(sf::RenderTarget & dest);

		bool handleClick(int x, int y);
	private:
		int _columnsNbr;
		int _padding;
		std::vector<TableCell*> _elements;
	};
}


#endif