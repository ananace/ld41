#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <vector>

#include <cstdint>

namespace Minesweeper
{

class Level : public sf::Drawable, public sf::Transformable
{
public:
    Level(const sf::Vector2u& size = {9, 9}, unsigned int mineCount = 0);
    ~Level();

    void Reset();

    void Reveal(const sf::Vector2u& tile);
    void Mark(const sf::Vector2u& tile);

    bool IsMine(const sf::Vector2u& tile) const;
    int MineCount(const sf::Vector2u& tile) const;
    const sf::Vector2u& GetSize() const;

    bool IsLost() const;
    bool IsWon() const;

    int TotalMineCount() const;
    int MarkedMineCount() const;
    int RemainingMineCount() const;

    void draw(sf::RenderTarget&, sf::RenderStates) const;

private:
    enum
    {
        k_flagMine = 1 << 0,
        k_flagRevealed = 1 << 1,
        k_flagMarked = 1 << 2
    };
    void FloodFill(const sf::Vector2u& tile, std::vector<size_t>& checked);

    sf::Texture m_bombTexture;
    sf::Vector2u m_size;
    unsigned int m_mineCount;
    std::vector<uint8_t> m_field;
};

}
