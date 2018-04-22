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

    void Draw(sf::RenderTarget&);
    void DrawUI(sf::RenderTarget&);

private:
    Level m_level;
};

}
