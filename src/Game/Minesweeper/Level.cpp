#include "Level.hpp"
#include "../Application.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include <random>
#include <string>

using namespace Minesweeper;

Level::Level(const sf::Vector2u& size, unsigned int mineCount)
    : m_size(size)
    , m_mineCount(mineCount)
{
    Reset();
}

Level::~Level()
{

}

void Level::Reset()
{
    m_mines.clear();
    if (m_mines.size() != m_size.x * m_size.y)
        m_mines.resize(m_size.x * m_size.y);
    for (size_t it = 0; it < m_mines.size(); ++it)
        m_mines[it] = false;
    if (m_revealed.size() != m_size.x * m_size.y)
        m_revealed.resize(m_size.x * m_size.y);
    for (size_t it = 0; it < m_revealed.size(); ++it)
        m_revealed[it] = false;

    std::random_device dev;
    std::uniform_int_distribution<> dist(0, (m_size.x * m_size.y) - 1);

    unsigned int mineCount = m_mineCount;
    if (mineCount == 0 || mineCount >= (m_size.x * m_size.y) / 2)
        mineCount = (m_size.x * m_size.y) / 7;
    unsigned int placed = 0;
    do
    {
        auto pos = dist(dev);
        if (m_mines[pos])
            continue;

        m_mines[pos] = true;
        ++placed;
    } while (placed < mineCount);
}

void Level::Reveal(const sf::Vector2u& tile)
{
    if (tile.x >= m_size.x || tile.y >= m_size.y)
        return;

    m_revealed[tile.x + tile.y * m_size.x] = true;

    if (MineCount(tile) == 0)
    {
        std::vector<size_t> checked;
        checked.reserve(10);
        FloodFill(tile, checked);
    }
}

bool Level::IsMine(const sf::Vector2u& tile) const
{
    if (tile.x >= m_size.x || tile.y >= m_size.y)
        return false;

    return m_mines.at(tile.x + tile.y * m_size.x);
}

int Level::MineCount(const sf::Vector2u& tile) const
{
    if (tile.x >= m_size.x || tile.y >= m_size.y)
        return 0;

    if (m_mines.at(tile.x + tile.y * m_size.x))
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

void Level::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    states.transform *= getTransform();

    sf::RectangleShape background({ m_size.x * 64.f, m_size.y * 64.f });
    background.setFillColor(sf::Color::Red);
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
    dropEffect.append(sf::Vertex({63, 1}, sf::Color(0xe1, 0xe1, 0xe1)));
    dropEffect.append(sf::Vertex({63, 63}, sf::Color(0xe1, 0xe1, 0xe1)));
    dropEffect.append(sf::Vertex({63, 63}, sf::Color(0xe1, 0xe1, 0xe1)));
    dropEffect.append(sf::Vertex({1, 63}, sf::Color(0xe1, 0xe1, 0xe1)));

    dropEffect.append(sf::Vertex({2, 62}, sf::Color(0xff, 0xff, 0xff)));
    dropEffect.append(sf::Vertex({2, 2}, sf::Color(0xff, 0xff, 0xff)));
    dropEffect.append(sf::Vertex({2, 2}, sf::Color(0xff, 0xff, 0xff)));
    dropEffect.append(sf::Vertex({62, 2}, sf::Color(0xff, 0xff, 0xff)));
    dropEffect.append(sf::Vertex({62, 2}, sf::Color(0xe1, 0xe1, 0xe1)));
    dropEffect.append(sf::Vertex({62, 62}, sf::Color(0xe1, 0xe1, 0xe1)));
    dropEffect.append(sf::Vertex({62, 62}, sf::Color(0xe1, 0xe1, 0xe1)));
    dropEffect.append(sf::Vertex({2, 62}, sf::Color(0xe1, 0xe1, 0xe1)));

    sf::Vector2f startPos(32.f, 32.f);
    for (size_t i = 0; i < m_revealed.size(); ++i)
    {
        sf::Vector2u tile(i % m_size.x, i / m_size.x);

        boxShape.setFillColor({ 0xf0, 0xf0, 0xf0 });
        boxShape.setPosition(tile.x * 64, tile.y * 64);
        boxShape.move(startPos);

        if (m_revealed[i] && m_mines[i])
            boxShape.setFillColor({ 0xff, 0x00, 0x00 });

        rt.draw(boxShape, states);

        if (!m_revealed[i])
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

        if (m_revealed[i])
            rt.draw(revealText, states);
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

    if (m_mines[offset])
        return;

    m_revealed[offset] = true;
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
