#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <vector>

namespace Minesweeper
{

class Level : public sf::Drawable, public sf::Transformable
{
public:
    Level(const sf::Vector2u& size = {9, 9}, unsigned int mineCount = 0);
    ~Level();

    void Reset();

    void Reveal(const sf::Vector2u& tile);
    bool IsMine(const sf::Vector2u& tile) const;
    int MineCount(const sf::Vector2u& tile) const;

    void draw(sf::RenderTarget&, sf::RenderStates) const;

private:
    sf::Vector2u m_size;
    unsigned int m_mineCount;
    std::vector<bool> m_mines;
    std::vector<bool> m_revealed;
};

}
