#include "Level.hpp"
#include "../Application.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <iostream>
#include <random>
#include <string>

using namespace Minesweeper;

Level::Level(const sf::Vector2u& size, unsigned int mineCount)
    : m_size(size)
    , m_mineCount(mineCount)
{
    Reset();

    const static std::string paths[] = {
#if defined(_WIN32)
        "resources\\bomb.png"
#else
        "resources/bomb.png",
        "/usr/share/LD41/resources/bomb.png",
        "/usr/local/share/LD41/resources/bomb.png",
        "/app/share/LD41/resources/bomb.png",
#endif
    };

    auto it = std::find_if(std::begin(paths), std::end(paths), [=](const std::string& file) { return m_bombTexture.loadFromFile(file); });
    if (it == std::end(paths))
        std::cout << "Failed to load bomb texture, continuing anyway" << std::endl;
}

Level::~Level()
{

}

void Level::Reset()
{
    if (m_field.size() != m_size.x * m_size.y)
        m_field.resize(m_size.x * m_size.y, 0);
    for (auto& i : m_field)
        i = 0;

    std::random_device dev;
    std::uniform_int_distribution<> dist(0, (m_size.x * m_size.y) - 1);

    unsigned int mineCount = m_mineCount;
    if (mineCount == 0 || mineCount >= (m_size.x * m_size.y) / 2)
        mineCount = (m_size.x * m_size.y) / 7;
    unsigned int placed = 0;
    do
    {
        auto pos = dist(dev);
        if (m_field[pos] & k_flagMine)
            continue;

        m_field[pos] |= k_flagMine;
        ++placed;
    } while (placed < mineCount);
}

void Level::Reveal(const sf::Vector2u& tile)
{
    if (tile.x >= m_size.x || tile.y >= m_size.y)
        return;

    if (m_field[tile.x + tile.y * m_size.x] & (k_flagRevealed | k_flagMarked))
        return;

    m_field[tile.x + tile.y * m_size.x] |= k_flagRevealed;

    if (MineCount(tile) == 0)
    {
        std::vector<size_t> checked;
        checked.reserve(10);
        FloodFill(tile, checked);
    }
}

void Level::Mark(const sf::Vector2u& tile)
{
    if (tile.x >= m_size.x || tile.y >= m_size.y)
        return;

    if (m_field[tile.x + tile.y * m_size.x] & k_flagRevealed)
        return;

    if (m_field[tile.x + tile.y * m_size.x] & k_flagMarked)
        m_field[tile.x + tile.y * m_size.x] &= ~k_flagMarked;
    else
        m_field[tile.x + tile.y * m_size.x] |= k_flagMarked;
}

bool Level::IsMine(const sf::Vector2u& tile) const
{
    if (tile.x >= m_size.x || tile.y >= m_size.y)
        return false;

    return m_field.at(tile.x + tile.y * m_size.x) & k_flagMine;
}

int Level::MineCount(const sf::Vector2u& tile) const
{
    if (tile.x >= m_size.x || tile.y >= m_size.y)
        return 0;

    if (m_field.at(tile.x + tile.y * m_size.x) & k_flagMine)
        return -1;

    int count = 0;
    count += IsMine({ tile.x + 1, tile.y + 0});
    count += IsMine({ tile.x + 1, tile.y + 1 });
    count += IsMine({ tile.x + 0, tile.y + 1 });
    count += IsMine({ tile.x - 1, tile.y + 1});
    count += IsMine({ tile.x - 1, tile.y + 0 });
    count += IsMine({ tile.x + 0, tile.y - 1 });
    count += IsMine({ tile.x + 1, tile.y - 1 });
    count += IsMine({ tile.x - 1, tile.y - 1});
    return count;
}

const sf::Vector2u& Level::GetSize() const
{
    return m_size;
}

void Level::SetSize(const sf::Vector2u& size)
{
    m_size = size;
}

bool Level::IsLost() const
{
    return std::any_of(m_field.begin(), m_field.end(), [](uint8_t square) { return (square & (k_flagMine | k_flagRevealed)) == (k_flagMine | k_flagRevealed); });
}

bool Level::IsWon() const
{
    return std::none_of(m_field.begin(), m_field.end(), [](uint8_t square) {
        if ((square & k_flagMarked) != 0 && (square & k_flagMine) == 0)
            return true; // Can't win with excessive marking
        if ((square & k_flagMine) == 0)
            return false; // Ignore mine-less tiles

        return (square & (k_flagMine | k_flagMarked)) != (k_flagMarked | k_flagMine);
    });
}

int Level::TotalMineCount() const
{
    return int(std::count_if(m_field.begin(), m_field.end(), [](uint8_t square) { return square & k_flagMine; }));
}
int Level::MarkedMineCount() const
{
    return int(std::count_if(m_field.begin(), m_field.end(), [](uint8_t square) { return square & k_flagMarked; }));
}
int Level::RemainingMineCount() const
{
    return TotalMineCount() - MarkedMineCount();
}

void Level::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    states.transform *= getTransform();

    sf::RectangleShape background({ m_size.x * 65.75f + 1.75f, m_size.y * 65.75f + 1.75f });
    background.move(-1.75f, -1.75f);
    background.setFillColor({ 0x80, 0x80, 0x80 });
    rt.draw(background, states);

    sf::Text revealText("2", Application::GetSingleton().GetDefaultFont());
    {
        auto size = revealText.getLocalBounds();
        revealText.setOrigin((size.left + size.width) / 2.f, (size.top + size.height) / 2.f);
    }

    sf::RectangleShape boxShape({ 64, 64 });
    // boxShape.setOutlineColor(sf::Color::Black);
    // boxShape.setOutlineThickness(1);
    boxShape.setOrigin(32, 32);

    sf::VertexArray dropEffect(sf::Lines, 16);
    dropEffect.append(sf::Vertex({1, 63}, sf::Color(0xff, 0xff, 0xff)));
    dropEffect.append(sf::Vertex({1, 1}, sf::Color(0xff, 0xff, 0xff)));
    dropEffect.append(sf::Vertex({1, 1}, sf::Color(0xff, 0xff, 0xff)));
    dropEffect.append(sf::Vertex({63, 1}, sf::Color(0xff, 0xff, 0xff)));
    dropEffect.append(sf::Vertex({63, 1}, sf::Color(0xe0, 0xe0, 0xe0)));
    dropEffect.append(sf::Vertex({63, 63}, sf::Color(0xe0, 0xe0, 0xe0)));
    dropEffect.append(sf::Vertex({63, 63}, sf::Color(0xe0, 0xe0, 0xe0)));
    dropEffect.append(sf::Vertex({1, 63}, sf::Color(0xe0, 0xe0, 0xe0)));

    dropEffect.append(sf::Vertex({2, 62}, sf::Color(0xff, 0xff, 0xff)));
    dropEffect.append(sf::Vertex({2, 2}, sf::Color(0xff, 0xff, 0xff)));
    dropEffect.append(sf::Vertex({2, 2}, sf::Color(0xff, 0xff, 0xff)));
    dropEffect.append(sf::Vertex({62, 2}, sf::Color(0xff, 0xff, 0xff)));
    dropEffect.append(sf::Vertex({62, 2}, sf::Color(0xe0, 0xe0, 0xe1)));
    dropEffect.append(sf::Vertex({62, 62}, sf::Color(0xe0, 0xe0, 0xe0)));
    dropEffect.append(sf::Vertex({62, 62}, sf::Color(0xe0, 0xe0, 0xe0)));
    dropEffect.append(sf::Vertex({2, 62}, sf::Color(0xe0, 0xe0, 0xe0)));

    sf::VertexArray mineVerts(sf::Triangles, 6);

    sf::Vector2f startPos(32.f, 32.f);
    for (size_t i = 0; i < m_field.size(); ++i)
    {
        sf::Vector2u tile((unsigned int)i % m_size.x, (unsigned int)i / m_size.x);

        boxShape.setFillColor((m_field[i] & k_flagRevealed) ? sf::Color{ 0xe0, 0xe0, 0xe0 } : sf::Color{ 0xf0, 0xf0, 0xf0 });
        boxShape.setPosition(tile.x * 65.75f, tile.y * 65.75f);
        boxShape.move(startPos);

        if (m_field[i] & k_flagMarked)
            boxShape.setFillColor({ 0x80, 0x00, 0x00 });
        if ((m_field[i] & (k_flagMine | k_flagRevealed)) == (k_flagMine | k_flagRevealed))
            boxShape.setFillColor({ 0xff, 0x00, 0x00 });

        rt.draw(boxShape, states);

        if ((m_field[i] & k_flagRevealed) == 0)
        {
            sf::RenderStates revealstates(states);
            revealstates.transform *= boxShape.getTransform();
            rt.draw(dropEffect, revealstates);
        }

        int mineCount = MineCount(tile);
        revealText.setPosition(boxShape.getPosition());
        revealText.setString(std::to_string(mineCount));

        switch(mineCount)
        {
            case 1: revealText.setFillColor({ 0x00, 0x00, 0xff }); break;
            case 2: revealText.setFillColor({ 0x00, 0x81, 0x00 }); break;
            case 3: revealText.setFillColor({ 0xff, 0x13, 0x00 }); break;
            case 4: revealText.setFillColor({ 0x00, 0x00, 0x83 }); break;
            case 5: revealText.setFillColor({ 0x81, 0x05, 0x00 }); break;
            case 6: revealText.setFillColor({ 0x2a, 0x94, 0x94 }); break;
            case 7: revealText.setFillColor({ 0x00, 0x00, 0x00 }); break;
            case 8: revealText.setFillColor({ 0x80, 0x80, 0x80 }); break;

            default: revealText.setFillColor(sf::Color::Transparent); break;
        }

        if (m_field[i] & k_flagRevealed)
        {
            rt.draw(revealText, states);

            if (m_field[i] & k_flagMine)
            {
                sf::Sprite bomb(m_bombTexture);
                bomb.setOrigin(sf::Vector2f(m_bombTexture.getSize()) / 2.f);
                bomb.setPosition(boxShape.getPosition());
                bomb.setScale(2.f, 2.f);
                rt.draw(bomb, states);
            }
        }
    }
}

void Level::FloodFill(const sf::Vector2u& tile, std::vector<size_t>& checked)
{
    if (tile.x >= m_size.x || tile.y >= m_size.y)
        return;

    size_t offset = tile.x + m_size.x * tile.y;

    if (std::find(checked.begin(), checked.end(), offset) != checked.end())
        return;
    checked.push_back(offset);

    if (m_field[offset] & k_flagMine)
        return;

    m_field[offset] |= k_flagRevealed;
    m_field[offset] &= ~k_flagMarked;

    if (MineCount(tile) != 0)
        return;

    if (tile.x < m_size.x -1)
        FloodFill({ tile.x + 1, tile.y + 0}, checked);
    if (tile.y < m_size.y - 1)
    {
        if (tile.x < m_size.x -1)
            FloodFill({ tile.x + 1, tile.y + 1 }, checked);
        FloodFill({ tile.x + 0, tile.y + 1 }, checked);
    }
    if (tile.x > 0)
    {
        if (tile.y < m_size.y - 1)
            FloodFill({ tile.x - 1, tile.y + 1}, checked);
        FloodFill({ tile.x - 1, tile.y + 0 }, checked);
    }
    if (tile.y > 0)
    {
        FloodFill({ tile.x + 0, tile.y - 1 }, checked);
        if (tile.x < m_size.x -1)
            FloodFill({ tile.x + 1, tile.y - 1 }, checked);
    }
    if (tile.x > 0 && tile.y > 0)
        FloodFill({ tile.x - 1, tile.y - 1}, checked);
}
