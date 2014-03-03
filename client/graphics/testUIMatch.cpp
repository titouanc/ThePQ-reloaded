#include "UIMatch.hpp"
#include <SFML/Graphics.hpp>
#include <model/Player.hpp>
#include <model/Displacement.hpp>
#include <toolbox.hpp>

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

                /* Interrupt move with escape key */
                if (isEscape(ev)){
                    stopped = true;
                } 

                else if (ev.type == sf::Event::MouseButtonPressed){
                    const Position click(ev.mouseButton.x, ev.mouseButton.y);
                    Position const & pos = ui.GUI2pitch(click);
                    Position const & delta = pos - currentPos;

                    if (delta == Position(0, 0)){
                        stopped = true;
                    } else if (
                        delta.length() <= steps && /* Position accessible with player's speed */
                        delta.isDirection() && /* Movement is a valid direction */
                        ui.pitch().inEllipsis(pos) && /* Destination is in ellipsis */
                        ! ui.pitch().getAt(pos) /* No moveable at this position */
                    ){
                        res.addMove(delta);
                        currentPos = pos;
                        steps -= delta.length();
                        ui.clear();
                        ui.hilightAccessibles(currentPos, steps);
                        ui.hilightDisplacement(pitchPos, res);
                        win.draw(ui);
                        win.display();
                    }
                }
            }
            if (res.count() > 0){
                JSON::Dict toSend = {
                    {"mid", JSON::Integer(player.getID())},
                    {"move", JSON::List(res)}
                };
                cout << toSend << endl;
                ui.pitch().setAt(pitchPos, NULL);
                ui.pitch().setAt(pitchPos + res.position(), atPos);
            }
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
                minisleep(0.5);
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
