#ifndef GUI_MAINCONTROLLER_HPP
#define GUI_MAINCONTROLLER_HPP

#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <stack>
#include "Layer.hpp"
#include "GUIConstants.hpp"

namespace GUI {
	class MainController {
	public:
		MainController();
		~MainController();
		void addLayer(Layer& layer);
		void deleteTopLayer();
		void renderTopLayer();
		bool handleClick(sf::Event e);
		bool handleRightClick(sf::Event e);
		void handleTextEntered(sf::Event e);
		void handleMouseMoved(sf::Event e);
		sf::RenderWindow window;
		sf::Music mainMusic;
	private:
		std::stack<Layer*> _layers;
	};
}

#endif