#include "Game.hpp"

#include <Application.hpp>
#include <Inputs.hpp>

#include <SFML/Graphics/RectangleShape.hpp>

using namespace Minesweeper;

Game::Game()
    : m_pressLength(0)
{
    
}
Game::~Game()
{
    
}

void Game::Update()
{
    float dt = Application::GetSingleton().GetStateManager().GetFrameTimeDT();
    auto& inp = Application::GetSingleton().GetInputManager();

    sf::Vector2i delta(inp[Input_Right].IsPressStart() - inp[Input_Left].IsPressStart(), inp[Input_Down].IsPressStart() - inp[Input_Up].IsPressStart());
    sf::Vector2i newCursor(m_cursor);
    newCursor += delta;

    sf::Vector2i lsize(m_level.GetSize());
    if (!sf::IntRect({0, 0}, lsize).contains(newCursor))
    {
        if (newCursor.x < 0 || newCursor.x >= lsize.x)
            newCursor.x = lsize.x - abs(newCursor.x);
        if (newCursor.y < 0 || newCursor.y >= lsize.y)
            newCursor.y = lsize.y - abs(newCursor.y);
    }
    m_cursor = sf::Vector2u(newCursor);

    if (inp[Input_Action].IsPressStart())
        m_pressLength = 0;
    if (inp[Input_Action].IsPressed())
        m_pressLength += dt;
    if (inp[Input_Action].IsPressStop())
    {
        if (m_pressLength <= 0.2f)
            m_level.Reveal(m_cursor);
        else
            m_level.Mark(m_cursor);
    }
}

void Game::Draw(sf::RenderTarget& rt)
{
    sf::RectangleShape background(rt.getView().getSize());
    background.setFillColor(sf::Color::White);
    rt.draw(background);

    sf::RectangleShape cursor({ 64, 64 });
    cursor.setFillColor({ 255, 255, 255, 128 });
    cursor.setOutlineColor(sf::Color::Black);
    cursor.setOutlineThickness(3.f);
    cursor.setPosition(sf::Vector2f(m_cursor) * 65.75f);

    auto view = rt.getView();
    view.setSize(view.getSize().x * 2.f, view.getSize().y);
    view.setCenter(cursor.getPosition() + sf::Vector2f(view.getSize().x / 4.f, 0));
    rt.setView(view);

    rt.draw(m_level);

    rt.draw(cursor);
}
void Game::DrawUI(sf::RenderTarget& rt)
{
    
}
