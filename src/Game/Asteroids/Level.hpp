#pragma once

namespace sf { class View; }

#include <SFML/Graphics/Drawable.hpp>

namespace Asteroids
{

class Level : public sf::Drawable
{
public:
    Level();
    ~Level();

    void Update();
    void Reset();

    /// \note The name is for sf::Drawable
    void draw(sf::RenderTarget& rt, sf::RenderStates states) const;

private:

};

}
