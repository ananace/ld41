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

    void Draw(sf::RenderTarget&) const;
    void DrawUI(sf::RenderTarget&) const;

private:
    Level m_level;
    sf::Vector2u m_cursor;
    float m_pressLength;
};

}
