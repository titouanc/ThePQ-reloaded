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
const sf::Color UIMatch::hilightBlue(0x00, 0x00, 0xff, ALPHA);

UIMatch::UIMatch(Pitch & pitch, const Squad & viewerSquad, int hexagonSize) : 
    sf::Drawable(),
    _pitch(pitch), 
    _ownSquad(viewerSquad),
    _size(hexagonSize), 
    _hexagon(circleSize(), 6), /* 6 sides regular polygon */
    _left(0), _top(0)
{
    sf::Texture *toLoad[] = {
        &_grass_texture, &_sand_texture, &_goal_texture, &_bludger_texture,
        &_quaffle_texture, &_snitch_texture,
        &_own_chaser_texture, &_own_seeker_texture, &_own_keeper_texture, &_own_beater_texture,
        &_other_chaser_texture, &_other_seeker_texture, &_other_keeper_texture, &_other_beater_texture,
        &_own_chaser_quaffle_texture, &_other_chaser_quaffle_texture, &_background_texture
    };
    const char *files[] = {
        "grass1.png", "sand1.png", "goal2_50.png", "Bludger.png", "Quaffle.png",
        "GoldenSnitch.png", "BluePlayer.png", "YellowPlayer.png", "GreenPlayer.png", "RedPlayer.png",
        "BlueStripedPlayer.png", "YellowStripedPlayer.png", "GreenStripedPlayer.png", "RedStripedPlayer.png",
        "BluePlayerWithQuaffle.png", "BlueStripedPlayerWithQuaffle.png", "StarBack.png"
    };
    for (int i=0; i<sizeof(toLoad)/sizeof(sf::Texture*); i++){
        if (! toLoad[i]->loadFromFile(texturePath(files[i])))
            throw TextureNotFound(files[i]);
    }
}

void UIMatch::setOwnSquadDirection(){
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
                bool isOfOwnTeam = _ownSquad.hasPlayer(&(Moveable &)player);
                if (player.isBeater()){
                    if (isOfOwnTeam)
                        playerSprite.setTexture(_own_beater_texture);
                    else
                        playerSprite.setTexture(_other_beater_texture);
                }
                else if (player.isSeeker()){
                    if (isOfOwnTeam){
                        playerSprite.setTexture(_own_seeker_texture);
                    }
                    else {
                        playerSprite.setTexture(_other_seeker_texture);
                    }
                }
                else if (player.isChaser()){
                    Chaser const & chaser = (Chaser const &) player;
                    if (isOfOwnTeam){
                        if (chaser.hasQuaffle())
                            playerSprite.setTexture(_own_chaser_quaffle_texture);
                        else
                            playerSprite.setTexture(_own_chaser_texture);
                    }
                    else {
                        if (chaser.hasQuaffle())
                            playerSprite.setTexture(_other_chaser_quaffle_texture);
                        else
                            playerSprite.setTexture(_other_chaser_texture);
                    }
                }
                else if (player.isKeeper()){
                    if (isOfOwnTeam)
                        playerSprite.setTexture(_own_keeper_texture);
                    else
                        playerSprite.setTexture(_other_keeper_texture);
                }
                s = _own_chaser_texture.getSize();
                rx = (double)_size/s.x;
                ry = (double)_size/s.y;
                if (    ( (isOfOwnTeam)  && (!_playsOnLeftSide) )
                    ||  ( (!isOfOwnTeam) && (_playsOnLeftSide) ) )
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
    sf::Sprite background(_background_texture);
    dest.draw(background);

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
