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
		int _size;
		sf::Texture _grass_texture, _sand_texture, _goal_texture;
		sf::CircleShape _grass, _sand;
		sf::RectangleShape _overlay;
		sf::Sprite _goal;
		double vOffset(void) const {return H_VOFF*_size;}
		double circleSize(void) const {return H_SQ2C*_size;}
	public:
		GraphicPitch(Pitch & pitch, unsigned cell_size=25) : 
		_pitch(pitch), _size(cell_size), _grass(circleSize(), 6), 
		_sand(circleSize(), 6), _overlay(sf::Vector2f(cell_size, cell_size)) {
			if (! _grass_texture.loadFromFile("textures/grass1.png"))
				throw "File not found !";
			_grass.setTexture(&_grass_texture);

			_grass.setTextureRect(sf::IntRect(0, 0, _size, _size));
			if (! _sand_texture.loadFromFile("textures/sand1.png"))
				throw "File not found !";
			_sand.setTexture(&_sand_texture);
			_grass.setTextureRect(sf::IntRect(0, 0, _size, _size));

			if (! _goal_texture.loadFromFile("textures/goal2_50.png"))
				throw "File not found!";
			_goal_texture.setSmooth(true);
			_goal.setTexture(_goal_texture);
			_goal.setScale(	(float)_size/_goal_texture.getSize().x, 
							(float)_size/_goal_texture.getSize().y);
			_overlay.setFillColor(sf::Color(0xcc, 0, 0, 0x42));
		}
		int width() const {return _pitch.width()*_size/2;}
		int height() const {return _pitch.height()*vOffset();}
		void renderTo(sf::RenderTarget & dest){
			int xwin=0, ywin=0;
			for (int y=_pitch.ymax()-1; y>=_pitch.ymin(); y--){
				xwin = (abs(y)%2)*_size/2;
				for (int x=_pitch.xmin(); x<_pitch.xmax(); x++){
					if (! _pitch.isValid(x, y)) 
						continue; /* Skip position that arent valid */
					if (_pitch.inEllipsis(x, y)){
						Position const & destpos = game2GUI(x, y);
						if (_pitch.isInKeeperZone(x, y)){
							_sand.setPosition(destpos.x(), destpos.y());
							dest.draw(_sand);
							if (_pitch.isGoal(x, y)){
								_goal.setPosition(destpos.x(), destpos.y());
								dest.draw(_goal);
							}
						} else {
							_grass.setPosition(destpos.x(), destpos.y());
							dest.draw(_grass);
						}
					}
					xwin += _size;
				}
				ywin += vOffset();
			}
			drawOverlay(dest, game2GUI(0, 0));
			drawOverlay(dest, game2GUI(_pitch.xmin(), 0));
			drawOverlay(dest, game2GUI(_pitch.xmax()-1, 0));
			drawOverlay(dest, game2GUI(0, _pitch.ymin()));
			drawOverlay(dest, game2GUI(0, _pitch.ymax()-1));
		}

		/* Translate a position in GUI coordinate system to game coordinates
		   where the upper left corner of the pitch is at (left,top) in window */
		Position GUI2game(Position const & pos){
			int x = (2*pos.x() - (width()-_size)/2)/_size;
			int y = (pos.y() - (height()-vOffset())/2)/vOffset();
			return Position(round(x), round(y));
		}
		Position GUI2game(int x, int y, int left=0, int top=0){
			return GUI2game(Position(x, y)) - Position(left, top);
		}
		Position game2GUI(Position const & pos){
			int x = (width()-_size)/2 + pos.x()*_size/2;
			int y = (height()-vOffset())/2 - pos.y()*vOffset();
			cout << pos.toJson() << " -> " << x << ", " << y << endl;
			return Position(round(x), round(y));
		}
		Position game2GUI(int x, int y, int left=0, int top=0){
			return game2GUI(Position(x, y)) + Position(left, top);
		}

		void drawOverlay(sf::RenderTarget & dest, int x, int y){
			_overlay.setPosition(x, y);
			dest.draw(_overlay);
		}
		void drawOverlay(sf::RenderTarget & dest, Position const & pos){
			drawOverlay(dest, pos.x(), pos.y());
		}

		void onClick(sf::RenderTarget & dest, int x, int y){
			Position arg(x, y);
			Position const & clicked = game2GUI(GUI2game(arg));
			Position const & delta = clicked-arg;

			drawOverlay(dest, clicked);
			if (abs(delta.x()) > _size || abs(delta.y()) > vOffset()){
				JSON::Dict res = {
					{"event", arg.toJson()},
					{"gamepos", GUI2game(x, y).toJson()},
					{"clicked", clicked.toJson()},
					{"delta", delta.toJson()}
				};
				cout << res << endl;
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
		if (hasChanged){
			window.display();
			hasChanged = false;
		}
		sf::Event ev;
		window.waitEvent(ev);
		if (
			(ev.type == sf::Event::Closed) ||
			(ev.type==sf::Event::KeyPressed && ev.key.code==sf::Keyboard::Escape)
		)
			window.close();

		else if (ev.type == sf::Event::MouseButtonPressed){
			repr.onClick(window, ev.mouseButton.x, ev.mouseButton.y);
			hasChanged = true;
		}
	}

	return EXIT_SUCCESS;
}
