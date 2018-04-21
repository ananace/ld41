#pragma once

#include "Level.hpp"

namespace Minesweeper
{

class Game
{
public:
    Game();
    ~Game();
    
private:
    Level m_level;
    sf::Vector2u m_cursor;
};

}
