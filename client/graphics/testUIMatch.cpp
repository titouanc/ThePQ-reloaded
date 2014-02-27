#include "UIMatch.hpp"
#include <SFML/Graphics.hpp>
#include <model/Player.hpp>

using namespace std;

int main(int argc, const char **argv)
{
	Pitch myPitch;
	UIMatch match(myPitch);
	sf::RenderWindow window(sf::VideoMode(1280, 720), "This is a test !");

	Seeker harry;
	myPitch.setAt(-12, 2, &harry);
	match.drawMoveables();

	window.clear(sf::Color::Yellow);
	window.draw(match);
	window.display();

	while (window.isOpen()){
		sf::Event ev;
		window.waitEvent(ev);
		if (ev.type == sf::Event::Closed)
			window.close();

		else if (ev.type == sf::Event::MouseButtonPressed){
			const Position click(ev.mouseButton.x, ev.mouseButton.y);

			if (match.isInBounds(click)){
				Position const & pos = match.GUI2pitch(click);
				match.drawMoveables();
				match.hilight(pos);
				window.draw(match);
				window.display();
			}
		}

		else if (ev.type == sf::Event::KeyPressed){
			switch (ev.key.code){
				case sf::Keyboard::Escape:
					window.close();
					break;
				case sf::Keyboard::Space:
					window.draw(match);
					window.display();
					break;
				default:
					break;
			}
		}
	}

	return 0;
}
