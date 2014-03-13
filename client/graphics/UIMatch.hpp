#ifndef DEFINE_UIMATCH_HEADER
#define DEFINE_UIMATCH_HEADER

#include <model/Pitch.hpp>
#include <model/PosMatrix.hpp>
#include <model/Displacement.hpp>
#include <model/Squad.hpp>
#include <SFML/Graphics.hpp>

class UIMatch : public sf::Drawable {
    private:
        Pitch & _pitch;             /* Represented pitch */
        const Squad & _ownSquad;    /* The squad of the player viewing the game */
        bool _playsOnLeftSide;      /* Whether the player starts on the left side of the pitch or not */
        unsigned int _size;         /* Width of an hexagon */
        sf::CircleShape _hexagon;   /* hilight shape */
        /* Upperleft corner position */
        int _left, _top;
        /* graphical elements */
        sf::Texture _grass_texture, _sand_texture, _goal_texture;
        sf::Texture _bludger_texture, _quaffle_texture, _snitch_texture;
        sf::Texture _own_chaser_texture, _own_seeker_texture, _own_beater_texture, _own_keeper_texture;
        sf::Texture _other_chaser_texture, _other_seeker_texture, _other_beater_texture, _other_keeper_texture;
        sf::Sprite _bludger, _quaffle, _snitch;
        /* Hilights matrix */
        PosMatrix<const sf::Color> _hilights;

        /* HELPERS */
        /* Return the radius of a circle enclosing an hexagon of _size */
        double circleSize(void) const;

        /* Return the distance between 2 lines of hexagons, in pixels */
        double vAlign(void) const;

        void drawMoveables(sf::RenderTarget & target) const;
        void drawHighlights(sf::RenderTarget & target) const;
    public:
        static const sf::Color hilightYellow, hilightRed, hilightBlue;

        /* Create a new UIMatch bound to pitch, where the distance between
           two borders of an hexagon is hexagonSize pixels */
        UIMatch(Pitch & pitch, const Squad & viewerSquad, int hexagonSize=25);

        /* Sets the side on which your own squad plays */
        void setOwnSquadDirection();

        /* Return graphical pitch width, in pixels */
        unsigned int width(void) const;
        /* Return graphical pitch height, in pixels */
        unsigned int height(void) const;

        Pitch & pitch(void) const;

        void setPosition(int left, int top);

        /* return pos (given in pitch coordinates) in GUI coordinate system */
        Position pitch2GUI(Position const & pos) const;
        /* return pos (given in GUI coordinates) in pitch coordinate system */
        Position GUI2pitch(Position const & pos) const;

        /* Is x,y (in pixels left-top to right-bottom) in pitch ? */
        bool isInBounds(int x, int y) const;
        bool isInBounds(Position const & pos) const;

        /* Conform to Drawable interface */
        void draw(sf::RenderTarget &target, sf::RenderStates states=sf::RenderStates()) const;

        void hilight(Position const & pos, const sf::Color *color=&hilightYellow);
        void hilightAccessibles(Position const & from, int len);
        void hilightDisplacement(Position const & from, Displacement const & move);

        void clear(void);
};

#endif
