#include "Game.hpp"

#include <Application.hpp>
#include <Inputs.hpp>
#include <Math.hpp>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

using namespace Minesweeper;

Game::Game()
    : m_pressLength(0)
    , m_time(0)
    , m_size(9)
{
    
}
Game::~Game()
{
    
}

void Game::Update()
{
    float dt = Application::GetSingleton().GetStateManager().GetFrameTimeDT();
    auto& inp = Application::GetSingleton().GetInputManager();

    m_time += dt;

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

void Game::Reset()
{
    m_level.SetSize(sf::Vector2u(m_size, m_size));
    m_level.Reset();
    m_time = 0;
}

bool Game::IsOver() const
{
    return m_level.IsLost() || m_level.IsWon();
}

bool Game::IsWon() const
{
    return m_level.IsWon();
}

float Game::GetTime() const
{
    return m_time;
}

int Game::GetCurSize() const
{
    return m_level.GetSize().x;
}

int Game::GetSize() const
{
    return m_size;
}

void Game::SetSize(int size)
{
    m_size = size;
}

void Game::Draw(sf::RenderTarget& rt) const
{
    sf::RectangleShape background(rt.getView().getSize());
    background.setFillColor({ 0xe0, 0xe0, 0xe0 });
    rt.draw(background);

    sf::RectangleShape cursor({ 64, 64 });
    cursor.setFillColor({ 255, 255, 255, 128 });
    cursor.setOutlineColor(sf::Color::Black);
    cursor.setOutlineThickness(3.f);
    cursor.setPosition(sf::Vector2f(m_cursor) * 65.75f);

    auto view = rt.getView();
    auto hvsize = view.getSize() / 4.f;
    auto lsize = sf::Vector2f(m_level.GetSize()) * 65.75f;
    sf::FloatRect viewLimit(hvsize.x, hvsize.y, lsize.x - hvsize.x * 2, lsize.y - hvsize.y * 2);

    view.setCenter(Math::constrain(viewLimit, cursor.getPosition() + sf::Vector2f(32, 32)));
    rt.setView(view);

    rt.draw(m_level);

    rt.draw(cursor);
}

void Game::DrawUI(sf::RenderTarget& rt) const
{
    sf::Text scoreText("Mines: " + std::to_string(m_level.RemainingMineCount()), Application::GetSingleton().GetDefaultFont());
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setCharacterSize(18u);
    scoreText.setOutlineColor(sf::Color::White);
    scoreText.setOutlineThickness(2.f);
    auto rect = scoreText.getLocalBounds();
    scoreText.setOrigin({ (rect.left + rect.width) / 2.f, (rect.top + rect.height) / 2.f });
    scoreText.setPosition({ rect.left + rect.width + 5, rect.top + rect.height + 5 });
    rt.draw(scoreText);

    scoreText.setString("Time: " + std::to_string(int(m_time)) + "s");
    rect = scoreText.getLocalBounds();
    scoreText.setOrigin({ (rect.left + rect.width) / 2.f, (rect.top + rect.height) / 2.f });
    scoreText.setPosition({ rt.getView().getSize().x - (rect.left + rect.width + 5), rect.top + rect.height + 5 });
    rt.draw(scoreText);
}
