#include "UIMatch.hpp"
#include "loadPath.hpp"
#include <stdexcept>
#include <model/Ball.hpp>
#include <model/Player.hpp>
#include "GUIConstants.hpp"
#include <cmath>

using namespace GUI;

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
const sf::Color UIMatch::hilightBlue(0x00, 0x00, 0xff, ALPHA);

struct TextureToLoad {sf::Texture & texture; const char *path;};

UIMatch::UIMatch(Pitch & pitch, const Squad & viewerSquad, int hexagonSize) : 
    sf::Drawable(),
    _pitch(pitch), 
    _ownSquad(viewerSquad),
    _size(hexagonSize), 
    _hexagon(circleSize(), 6), /* 6 sides regular polygon */
    _left(0), _top(0),
    _tooltip(" ", _tooltip_font, 10), _tooltip_visible(false)
{
    TextureToLoad toLoad[] = {
        /* Background */
        {_background_texture, "StarBack.png"},
        /* Pitch */
        {_grass_texture, "grass1.png"},
        {_sand_texture, "sand1.png"},
        {_goal_texture, "goal2_50.png"},
        /* Balls */
        {_bludger_texture, "Bludger.png"},
        {_quaffle_texture, "Quaffle.png"},
        {_snitch_texture, "GoldenSnitch.png"},
    };

    for (size_t i=0; i<sizeof(toLoad)/sizeof(TextureToLoad); i++){
        if (! toLoad[i].texture.loadFromFile(texturePath(toLoad[i].path)))
            throw TextureNotFound(toLoad[i].path);
    }
    _background.setTexture(_background_texture);

    const char *players[] = {"Yellow", "Red", "GreenQuaffle", "Green", "BlueQuaffle", "Blue"};
    for (int i=0; i<6; i++){
        std::string base(players[i]);
        if (! _own_players_textures[i].loadFromFile(texturePath(base+"Player.png")))
            throw TextureNotFound(base+"Player.png");
        if (! _other_players_textures[i].loadFromFile(texturePath(base+"StripedPlayer.png")))
            throw TextureNotFound(base+"StripedPlayer.png");
    }

    if (! _tooltip_font.loadFromFile(fontPath(BODY_FONT_PATH)))
        throw TextureNotFound(BODY_FONT_PATH);

    _tooltip.setColor(sf::Color::Black);
}

sf::Texture const & UIMatch::playerTexture(Player const & player) const
{
    /* Bank: [S, B, K*, K, C*, C] */
    const sf::Texture *bank = _own_players_textures;
    if (! _ownSquad.hasPlayer((Moveable *) &player))
        bank = _other_players_textures;
    if (player.isSeeker())
        return bank[0];
    if (player.isBeater())
        return bank[1];

    PlayerQuaffle & pq = (PlayerQuaffle &) player;
    if (pq.isKeeper())
        return (pq.hasQuaffle()) ? bank[2] : bank[3];
    return (pq.hasQuaffle()) ? bank[4] : bank[5];
}

void UIMatch::setOwnSquadDirection()
{
    if (_ownSquad.players[0]->getPosition().x() > 0)
        _playsOnLeftSide = false;
    else
        _playsOnLeftSide = true;
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
    double w=width(), h=height(), s=_size; /* Signed versions */
    double x = 1 + (2*relpos.x() - s/2 - w)/s;
    double y = ((h-vAlign())/2 - relpos.y())/vAlign();

    Position res = Position(x, (y > 0) ? ceil(y) : y);
    if (! _pitch.isValid(res))
        res = res - Position(1, 0);
    return res;
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
    sf::Sprite bludger(_bludger_texture);
    sf::Vector2u s = _bludger_texture.getSize();
    double rx = (double)_size/s.x;
    double ry = (double)_size/s.y;
    bludger.setScale(sf::Vector2f(rx, ry));

    sf::Sprite quaffle(_quaffle_texture);
    s = _quaffle_texture.getSize();
    rx = (double)_size/s.x;
    ry = (double)_size/s.y;
    quaffle.setScale(sf::Vector2f(rx, ry));

    sf::Sprite snitch(_snitch_texture);
    s = _snitch_texture.getSize();
    rx = (double)_size/s.x;
    ry = (double)_size/s.y;
    snitch.setScale(sf::Vector2f(rx, ry));

    if (_pitch.size() > 0){
        Pitch::const_iterator it;
        sf::Sprite playerSprite;

        for (it=_pitch.begin(); it!=_pitch.end(); it++){
            Position const & destpos = pitch2GUI(it->first);
            
            if (it->second->isBall()){
                Ball const & ball = (Ball const &) *(it->second);
                if (ball.isBludger()){
                    bludger.setPosition(destpos.x(), destpos.y());
                    dest.draw(bludger);
                }
                else if (ball.isGoldenSnitch()){
                    snitch.setPosition(destpos.x(), destpos.y());
                    dest.draw(snitch);
                }
                else if (ball.isQuaffle()){
                    quaffle.setPosition(destpos.x(), destpos.y());
                    dest.draw(quaffle);
                }

            } else if (it->second->isPlayer()){
                Player const & player = (Player const &) *(it->second);
                sf::Texture const & pTexture = playerTexture(player);
                playerSprite.setTexture(pTexture);
                bool inMySquad = _ownSquad.hasPlayer(&(Moveable &)player);
                
                s = pTexture.getSize();
                rx = (double)_size/s.x;
                ry = (double)_size/s.y;
                if (( inMySquad && !_playsOnLeftSide ) || ( !inMySquad && _playsOnLeftSide ))
                    rx = -rx;
                playerSprite.setScale(sf::Vector2f(rx, ry));
                if (rx<0)
                    playerSprite.setPosition(destpos.x()+_size, destpos.y());
                else
                    playerSprite.setPosition(destpos.x(), destpos.y());
                dest.draw(playerSprite);
            }
        }
    }
}

void UIMatch::draw(sf::RenderTarget &dest, sf::RenderStates states) const
{
    dest.draw(_background);

    sf::CircleShape sand(circleSize(), 6);
    //sand.setTexture(&_sand_texture);

    sf::CircleShape grass(circleSize(), 6);
    sand.setFillColor(sf::Color::White);
    //grass.setTexture(&_grass_texture);

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
                    sand.setOutlineColor(sf::Color(0x00, 0xae, 0xef, 0x44));
                    sand.setOutlineThickness(1);
                    dest.draw(sand);

                    if (_pitch.isGoal(pos)){
                        goal.setPosition(destpos.x(), destpos.y());
                        dest.draw(goal);
                    }
                } else {
                    grass.setPosition(destpos.x(), destpos.y());
                    int blue = 249;
                    int red = 184;
                    blue = blue-2*sqrt(abs(x))-3*sqrt(abs(y));
                    red = red-2*sqrt(abs(x))-3*abs(abs(y));
                    if (blue > 256) blue = 255;
                    grass.setFillColor(sf::Color(0x05, red, blue, 0xff));
                    dest.draw(grass);
                }
            }
        }
    }

    drawHighlights(dest);
    drawMoveables(dest);
    if (_tooltip_visible)
        dest.draw(_tooltip);
}

void UIMatch::showTooltip(Position const & pos, std::string const & text)
{
    if (text.length() == 0)
        _tooltip.setString(" "); /* sf::String bug on OSX */
    else
        _tooltip.setString(text);
    Position const & dest = pitch2GUI(pos);
    _tooltip.setPosition(dest.x(), dest.y());
    _tooltip_visible = true;
}

void UIMatch::hideTooltip()
{
    _tooltip_visible = false;
}

void UIMatch::hilight(Position const & pos, const sf::Color *color)
{
    _hilights.setAt(pos, color);
}

void UIMatch::clear(void)
{
    _hilights.clear();
}

void UIMatch::hilightAccessibles(Position const & from, int len, const sf::Color *color)
{
    for (int i=0; i<6; i++){
        for (int j=1; j<=len; j++){
            Position const & toDraw = from + Pitch::directions[i]*j;
            if (_pitch.inEllipsis(toDraw))
                hilight(toDraw, color);
        }
    }
}

void UIMatch::hilightDisplacement(Position const & from, Displacement const & move, const sf::Color *color)
{
    for (size_t i=0; i<=move.length(); i++){
        double t = ((double) i)/move.length();
        Position const & atTime = from + move.position(t);
        hilight(atTime, color);
    }
}
