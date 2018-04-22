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
    void Reset();

    bool IsOver() const;
    bool IsWon() const;
    float GetTime() const;

    void Draw(sf::RenderTarget&) const;
    void DrawUI(sf::RenderTarget&) const;

private:
    Level m_level;
    sf::Vector2u m_cursor;
    float m_pressLength, m_time;
};

}
