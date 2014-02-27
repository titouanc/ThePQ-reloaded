#include "UIMatch.hpp"
#include <stdexcept>

class TextureNotFound : public std::runtime_error {
    public: using std::runtime_error::runtime_error;
};

/* === Private === */
std::string UIMatch::texturePath(std::string const & name) const
{
    return std::string("textures/") + name + ".png";
}

void UIMatch::createBackground(void)
{
    sf::Texture grass_texture, sand_texture, goal_texture;
    if (! grass_texture.loadFromFile(texturePath("grass1")))
        throw TextureNotFound("grass1");
    if (! sand_texture.loadFromFile(texturePath("sand1")))
        throw TextureNotFound("sand1");
    if (! goal_texture.loadFromFile(texturePath("goal2_50")))
        throw TextureNotFound("goal2_50");

    sf::CircleShape sand(circleSize(), 6);
    sand.setTexture(&sand_texture);

    sf::CircleShape grass(circleSize(), 6);
    grass.setTexture(&grass_texture);

    sf::Sprite goal(goal_texture);
    sf::Vector2u goal_size(goal_texture.getSize());
    double rx = (double)_size/goal_size.x;
    double ry = (double)_size/goal_size.y;
    goal.setScale(sf::Vector2f(rx, ry));

    _bkg.create(width(), height());
    _bkg.clear(sf::Color(0, 0, 0, 0));

    for (int y=_pitch.ymax()-1; y>=_pitch.ymin(); y--){
        for (int x=_pitch.xmin(); x<_pitch.xmax(); x++){
            Position pos(x, y);
            if (! _pitch.isValid(pos)) 
                continue; /* Skip position that arent valid */

            if (_pitch.inEllipsis(pos)){
                Position const & destpos = pitch2GUI(pos);

                if (_pitch.isInKeeperZone(pos)){
                    sand.setPosition(destpos.x(), destpos.y());
                    _bkg.draw(sand);

                    if (_pitch.isGoal(pos)){
                        goal.setPosition(destpos.x(), destpos.y());
                        _bkg.draw(goal);
                    }
                } else {
                    grass.setPosition(destpos.x(), destpos.y());
                    _bkg.draw(grass);
                }
            }
        }
    }

    _bkg.display();
}

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
#define ALPHA 0x40
const sf::Color UIMatch::hilightYellow(0xcc, 0xcc, 0x00, ALPHA);
const sf::Color UIMatch::hilightRed(0xcc, 0x00, 0x00, ALPHA);

UIMatch::UIMatch(Pitch & pitch, int hexagonSize) : 
    sf::Drawable(),
    _pitch(pitch), 
    _size(hexagonSize), 
    _hexagon(circleSize(), 6), /* 6 sides regular polygon */
    _bkg(),
    _overlay(),
    _left(0), _top(0)
{
    _overlay.create(width(), height());
    _overlay.clear(sf::Color(0x00, 0x00, 0x00, 0x00));
    createBackground();
}

unsigned int UIMatch::width(void) const
{
    return _pitch.width()*_size/2;
}

unsigned int UIMatch::height(void) const
{
    return _pitch.height()*vAlign();
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

/* Conform to Drawable interface */
void UIMatch::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::Sprite bkg(_bkg.getTexture()), overlay(_overlay.getTexture());
    target.draw(bkg);
    target.draw(overlay);
}

/* Draw a colored hexagon at given position */
void UIMatch::hilight(Position const & pos, sf::Color const & color)
{
    Position const & GUIpos = pitch2GUI(pos);
    _hexagon.setFillColor(color);
    _hexagon.setPosition(GUIpos.x(), GUIpos.y());
    _overlay.draw(_hexagon);
    _overlay.display();
}
