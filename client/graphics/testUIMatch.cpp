#include "UIMatch.hpp"
#include <SFML/Graphics.hpp>
#include <model/Player.hpp>
#include <model/Displacement.hpp>

using namespace std;

static inline bool isEscape(sf::Event & ev)
{
    return (ev.type == sf::Event::KeyPressed && 
            ev.key.code == sf::Keyboard::Escape);
}

void handleClick(sf::RenderWindow & win, UIMatch & ui, Position const & pitchPos)
{
    Moveable *atPos = ui.pitch().getAt(pitchPos);
    if (atPos){
        if (atPos->isPlayer()){
            Player const & player = (Player const &) *atPos;

            Displacement res;
            Position currentPos = pitchPos;
            size_t steps = 5;//player.getSpeed();
            bool stopped = false;

            ui.hilightAccessibles(currentPos, steps);
            win.draw(ui);
            win.display();
            /* While player can still move */
            while (! stopped && steps > 0){
                sf::Event ev;
                win.waitEvent(ev);
                if (isEscape(ev)){
                    stopped = true;
                } else if (ev.type == sf::Event::MouseButtonPressed){
                    const Position click(ev.mouseButton.x, ev.mouseButton.y);
                    Position const & pos = ui.GUI2pitch(click);
                    Position const & delta = pos - currentPos;
                    if (delta.length() <= steps && delta.isDirection()){
                        res.addMove(delta);
                        currentPos = pos;
                        steps -= delta.length();
                        if (steps > 0){
                            ui.clear();
                            ui.hilightAccessibles(currentPos, steps);
                            ui.hilightDisplacement(pitchPos, res);
                            win.draw(ui);
                            win.display();
                        }
                    }
                }
            }
            cout << "Displacement: " << res.toJson() << endl;
        }
    }
}

int main(int argc, const char **argv)
{
    Pitch myPitch;
    Seeker harry(42, "Harry");
    myPitch.setAt(13, -3, &harry);

    UIMatch match(myPitch);
    sf::RenderWindow window(sf::VideoMode(1280, 720), "This is a test !");

    window.clear(sf::Color::White);
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
                handleClick(window, match, pos);
                match.clear();
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
