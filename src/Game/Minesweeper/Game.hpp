#pragma once

#include "Level.hpp"

namespace Minesweeper
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
    sf::Vector2u m_cursor;
};

}
