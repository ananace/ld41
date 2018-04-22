#pragma once

#include "Level.hpp"

namespace Asteroids
{

class Game
{
public:
    Game();
    ~Game();

    void Update();

    void Draw(sf::RenderTarget&) const;
    void DrawUI(sf::RenderTarget&) const;

private:
    Level m_level;
};

}
