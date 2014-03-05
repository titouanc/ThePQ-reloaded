#include "UIMatch.hpp"
#include "loadPath.hpp"
#include <stdexcept>
#include <model/Ball.hpp>
#include <model/Player.hpp>

class TextureNotFound : public std::runtime_error {
    public: using std::runtime_error::runtime_error;
};

double UIMatch::circleSize(void) const 
{
    /* (1/2)*(1/cos(30°)) */
    return 0.5773502691896257*_size;
}

double UIMatch::vAlign(void) const 
{
    /* (1 + tan(30°))/2 */
    return 0.7886751345948129*_size;
}

/* === Public === */
#define ALPHA 0x60
const sf::Color UIMatch::hilightYellow(0xcc, 0xcc, 0x00, ALPHA);
const sf::Color UIMatch::hilightRed(0xcc, 0x00, 0x00, ALPHA);

UIMatch::UIMatch(Pitch & pitch, int hexagonSize) : 
    sf::Drawable(),
    _pitch(pitch), 
    _size(hexagonSize), 
    _hexagon(circleSize(), 6), /* 6 sides regular polygon */
    _left(0), _top(0)
{
    
    if (! _grass_texture.loadFromFile(texturePath("grass1.png")))
        throw TextureNotFound("grass1");
    if (! _sand_texture.loadFromFile(texturePath("sand1.png")))
        throw TextureNotFound("sand1");
    if (! _goal_texture.loadFromFile(texturePath("goal2_50.png")))
        throw TextureNotFound("goal2_50");
}

unsigned int UIMatch::width(void) const
{
    return _pitch.width()*_size/2;
}

unsigned int UIMatch::height(void) const
{
    return _pitch.height()*vAlign();
}

Pitch & UIMatch::pitch(void) const
{
    return _pitch;
}

void UIMatch::setPosition(int left, int top)
{
    _left = left;
    _top = top;
}

Position UIMatch::GUI2pitch(Position const & pos) const
{
    Position const & relpos = pos - Position(_left, _top);
    int w=width(), h=height(), s=_size; /* Signed versions */
    int x = 1 + (2*relpos.x() - s/2 - w)/s;
    int y = ((h-vAlign())/2 - relpos.y())/vAlign();
    if (! _pitch.isValid(x, y)){
        x--;
    }
    return Position(x, y);
}

Position UIMatch::pitch2GUI(Position const & pos) const
{
    int w=width(), h=height(), s=_size; /* Signed versions */
    int x = (w-s)/2 + pos.x()*s/2;
    int y = (h-vAlign())/2 - pos.y()*vAlign();
    return Position(_left+x, _top+y);
}

bool UIMatch::isInBounds(int x, int y) const
{
    int right = _left + width();
    int bottom = _top + height();
    return (_left<=x && x<right && _top<=y && y<bottom);
}

bool UIMatch::isInBounds(Position const & pos) const
{
    return isInBounds(pos.x(), pos.y());
}

void UIMatch::drawHighlights(sf::RenderTarget & dest) const
{
    if (_hilights.size() > 0){
        PosMatrix<const sf::Color>::const_iterator it;
        sf::CircleShape shape(circleSize(), 6);

        for (it=_hilights.begin(); it!=_hilights.end(); it++){
            Position const & destpos = pitch2GUI(it->first);
            shape.setPosition(destpos.x(), destpos.y());
            shape.setFillColor(*(it->second));
            dest.draw(shape);
        }
    }
}

void UIMatch::drawMoveables(sf::RenderTarget & dest) const
{
    if (_pitch.size() > 0){
        Pitch::const_iterator it;
        sf::CircleShape shape(5*circleSize()/6);

        for (it=_pitch.begin(); it!=_pitch.end(); it++){
            Position const & destpos = pitch2GUI(it->first);
            shape.setPosition(destpos.x(), destpos.y());
            
            if (it->second->isBall()){
                Ball const & ball = (Ball const &) *(it->second);
                if (ball.isBludger())
                    shape.setFillColor(sf::Color(0xff, 0, 0, 0xff));
                else if (ball.isGoldenSnitch())
                    shape.setFillColor(sf::Color(0xcc, 0xcc, 0, 0xff));
                else if (ball.isQuaffle())
                    shape.setFillColor(sf::Color(0, 0, 0xff, 0xff));

            } else if (it->second->isPlayer()){
                Player const & player = (Player const &) *(it->second);
                if (player.isBeater())
                    shape.setFillColor(sf::Color(0xff, 0x33, 0x33, 0xff));
                else if (player.isSeeker())
                    shape.setFillColor(sf::Color(0xff, 0xff, 0x33, 0xff));
                else if (player.isChaser())
                    shape.setFillColor(sf::Color(0x33, 0, 0xff, 0xff));
                else if (player.isKeeper())
                    shape.setFillColor(sf::Color(0, 0xff, 0x33, 0xff));
            }
            dest.draw(shape);
        }
    }
}

void UIMatch::draw(sf::RenderTarget &dest, sf::RenderStates states) const
{
    sf::CircleShape sand(circleSize(), 6);
    sand.setTexture(&_sand_texture);

    sf::CircleShape grass(circleSize(), 6);
    grass.setTexture(&_grass_texture);

    sf::Sprite goal(_goal_texture);
    sf::Vector2u goal_size(_goal_texture.getSize());
    double rx = (double)_size/goal_size.x;
    double ry = (double)_size/goal_size.y;
    goal.setScale(sf::Vector2f(rx, ry));

    for (int y=_pitch.ymax()-1; y>=_pitch.ymin(); y--){
        for (int x=_pitch.xmin(); x<_pitch.xmax(); x++){
            Position pos(x, y);
            if (! _pitch.isValid(pos)) 
                continue; /* Skip position that arent valid */

            if (_pitch.inEllipsis(pos)){
                Position const & destpos = pitch2GUI(pos);

                if (_pitch.isInKeeperZone(pos)){
                    sand.setPosition(destpos.x(), destpos.y());
                    dest.draw(sand);

                    if (_pitch.isGoal(pos)){
                        goal.setPosition(destpos.x(), destpos.y());
                        dest.draw(goal);
                    }
                } else {
                    grass.setPosition(destpos.x(), destpos.y());
                    dest.draw(grass);
                }
            }
        }
    }

    drawHighlights(dest);
    drawMoveables(dest);
}

void UIMatch::hilight(Position const & pos, const sf::Color *color)
{
    _hilights.setAt(pos, color);
}

void UIMatch::clear(void)
{
    _hilights.clear();
}

void UIMatch::hilightAccessibles(Position const & from, int len)
{
    for (int i=0; i<6; i++){
        for (int j=1; j<=len; j++){
            Position const & toDraw = from + Pitch::directions[i]*j;
            if (_pitch.inEllipsis(toDraw))
                hilight(toDraw, &hilightYellow);
        }
    }
}

void UIMatch::hilightDisplacement(Position const & from, Displacement const & move)
{
    for (size_t i=0; i<=move.length(); i++){
        double t = ((double) i)/move.length();
        Position const & atTime = from + move.position(t);
        hilight(atTime, &hilightRed);
    }
}
