#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <model/Pitch.hpp>
#include <cmath>

#include <iostream>

using namespace std;

/* Ratio between square size and circle radius (both enclosing hexagon)
   (1/2)*(1/cos(30°)) */
#define H_SQ2C 0.5773502691896257
/* Vertical offset: coefficient between 2 lines of hexagons
   (1 + tan(30°))/2  */
#define H_VOFF 0.7886751345948129

class GraphicPitch {
	private:
		Pitch & _pitch;
		unsigned int _size;
		sf::Texture _grass_texture, _sand_texture, _goal_texture;
		sf::CircleShape _grass, _sand;
		sf::Sprite _goal;
	public:
		GraphicPitch(Pitch & pitch, unsigned cell_size=25) : 
		_pitch(pitch), _size(cell_size), _grass(H_SQ2C*_size, 6), 
		_sand(H_SQ2C*_size, 6){
			if (! _grass_texture.loadFromFile("textures/grass1.png"))
				throw "File not found !";
			_grass.setTexture(&_grass_texture);

			_grass.setTextureRect(sf::IntRect(0, 0, _size, _size));
			if (! _sand_texture.loadFromFile("textures/sand1.png"))
				throw "File not found !";
			_sand.setTexture(&_sand_texture);
			_grass.setTextureRect(sf::IntRect(0, 0, _size, _size));

			if (! _goal_texture.loadFromFile("textures/goal1_50.png"))
				throw "File not found!";
			_goal_texture.setSmooth(true);
			_goal.setTexture(_goal_texture);
			_goal.setScale(	(float)_size/_goal_texture.getSize().x, 
							(float)_size/_goal_texture.getSize().y);
		}
		void renderTo(sf::RenderTarget & dest){
			unsigned int xwin=0, ywin=0;
			for (int y=_pitch.ymin(); y<_pitch.ymax(); y++){
				xwin = (abs(y)%2)*_size/2;
				for (int x=_pitch.xmin(); x<_pitch.xmax(); x++){
					if (! _pitch.isValid(x, y)) 
						continue; /* Skip position that arent valid */
					if (_pitch.inEllipsis(x, y)){
						if (_pitch.isInKeeperZone(x, y)){
							_sand.setPosition(xwin, ywin);
							dest.draw(_sand);
							if (_pitch.isGoal(x, y)){
								_goal.setPosition(xwin, ywin);
								dest.draw(_goal);
							}
						} else {
							_grass.setPosition(xwin, ywin);
							dest.draw(_grass);
						}
					}
					xwin += _size;
				}
				ywin += H_VOFF*_size;
			}
		}
};

int main(int argc, const char **argv)
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "The Pro Quidditch");

	Pitch myPitch;
	GraphicPitch repr(myPitch);
	window.setFramerateLimit(5);

	window.clear(sf::Color(0xff, 0xff, 0xff, 0xff));
	try {
		repr.renderTo(window);
	} catch (const char *msg){
		cerr << msg << endl;
		return EXIT_FAILURE;
	}
	
	bool hasChanged = false;
	window.display();
	while (window.isOpen()){
		if (hasChanged)
			window.display();
		sf::Event ev;
		window.waitEvent(ev);
		if (
			(ev.type == sf::Event::Closed) ||
			(ev.type==sf::Event::KeyPressed && ev.key.code==sf::Keyboard::Escape)
		)
			window.close();
	}

	return EXIT_SUCCESS;
}
