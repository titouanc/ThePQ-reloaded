#ifndef DEFINE_UIMATCH_HEADER
#define DEFINE_UIMATCH_HEADER

#include <model/Pitch.hpp>
#include <SFML/Graphics.hpp>

class UIMatch : public sf::Drawable {
    private:
        Pitch & _pitch;             /* Represented pitch */
        unsigned int _size;         /* Width of an hexagon */
        sf::CircleShape _hexagon;   /* hilight shape */
        sf::RenderTexture _bkg;     /* Constant background */
        sf::RenderTexture _overlay; /* Variable foreground */
        bool _hasChanged;           /* The overlay has changed */

        /* Return the radius of a circle enclosing an hexagon of _size */
        double circleSize(void) const;

        /* Return the distance between 2 lines of hexagons, in pixels */
        double vAlign(void) const;

        /* Create background once; it will result in a 1-op surface blit
           gracefully handled by GPU */
        void createBackground(void);

        std::string texturePath(std::string const & name) const;

        /* return pos (given in pitch coordinates) in GUI coordinate system */
        Position pitch2GUI(Position const & pos) const;

        /* return pos (given in GUI coordinates) in pitch coordinate system */
        Position GUI2pitch(Position const & pos) const;
    public:
        static const sf::Color hilightYellow, hilightRed;

        /* Create a new UIMatch bound to pitch, where the distance between
           two borders of an hexagon is hexagonSize */
        UIMatch(Pitch & pitch, int hexagonSize=25);

        /* Return graphical pitch width, in pixels */
        unsigned int width(void) const;
        /* Return graphical pitch width, in pixels */
        unsigned int height(void) const;

        /* Conform to Drawable interface */
        void draw(sf::RenderTarget &target, sf::RenderStates states=sf::RenderStates()) const;

        /* Draw a colored hexagon at given position */
        void hilight(Position const & pos, sf::Color const & color=hilightRed);

        /* Return true if it should be redrawn */
        bool hasChanged(void) const;
};

#endif

/*
!!! FR76 3000 3022 2300 0505 2543 635 / SOGEFRPP !!!
*/