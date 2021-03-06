#include "GameState.hpp"

#include <Application.hpp>
#include <Inputs.hpp>
#include <StateManager.hpp>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>

GameState::GameState()
    : m_paused(false)
    , m_gameover(false)
    , m_won(false)
    , m_pauseTime(0)
    , m_cumulativePauseTime(0)
{

}

GameState::~GameState()
{

}

void GameState::Update()
{
    auto& inp = Application::GetSingleton().GetInputManager();

    if (!m_paused && !m_gameover)
    {
        m_asteroids.Update();
        m_minesweeper.Update();

        if (m_asteroids.IsOver())
        {
            m_gameover = true;
            m_reason = Reason_Asteroids;
        }
        if (m_minesweeper.IsOver())
        {
            m_gameover = true;
            m_won = m_minesweeper.IsWon();
            m_reason = Reason_Minesweeper;
        }
    }
    else
    {
        float dt = GetStateManager().GetFrameTimeDT();
        m_pauseTime += dt;
        if (m_paused)
            m_cumulativePauseTime += dt;
    }

    if (inp[Input_Cancel].IsPressStart())
    {
        if (m_gameover)
        {
            m_asteroids.Reset();
            m_minesweeper.Reset();

            m_paused = m_gameover = m_won = false;
            m_cumulativePauseTime = 0;
        }
        else
            m_paused = !m_paused;
    }

    if (m_gameover)
    {
        if (inp[Input_Up].IsPressStart())
            m_minesweeper.SetSize(m_minesweeper.GetSize() + 1);
        else if (inp[Input_Down].IsPressStart())
        {
            int size = m_minesweeper.GetSize();
            if (size > 5)
                m_minesweeper.SetSize(size - 1);
        }
    }
}

void GameState::Draw(sf::RenderTarget& rt) const
{
    auto preView = rt.getView();

    sf::View asView({ 0, 0, float(rt.getSize().x) / 2.f, float(rt.getSize().y) });
    asView.setViewport({ 0, 0, 0.5, 1 });
    sf::View msView({ 0, 0, float(rt.getSize().x) / 2.f, float(rt.getSize().y) });
    msView.setViewport({ 0.5, 0, 0.5, 1 });

    rt.setView(asView);
    m_asteroids.Draw(rt);

    rt.setView(msView);
    m_minesweeper.Draw(rt);

    rt.setView(preView);
}

void GameState::DrawUI(sf::RenderTarget& rt) const
{
    auto realView = rt.getView();
    sf::View asView({ 0, 0, float(rt.getSize().x) / 2.f, float(rt.getSize().y) });
    asView.setViewport({ 0, 0, 0.5, 1 });
    sf::View msView({ 0, 0, float(rt.getSize().x) / 2.f, float(rt.getSize().y) });
    msView.setViewport({ 0.5, 0, 0.5, 1 });

    rt.setView(asView);
    m_asteroids.DrawUI(rt);
    rt.setView(msView);
    m_minesweeper.DrawUI(rt);

    rt.setView(realView);
    if (m_paused || m_gameover)
    {
        sf::RectangleShape darken(rt.getView().getSize());
        darken.setFillColor({ 0, 0, 0, uint8_t(std::min(m_pauseTime, 1.f) * 128) });

        rt.draw(darken);
    }

    if (m_paused)
    {
        sf::Text pauseText("[ Paused ]", Application::GetSingleton().GetDefaultFont());
        pauseText.setCharacterSize(38u);
        pauseText.setOutlineColor(sf::Color::Black);
        pauseText.setOutlineThickness(2.f);
        auto rect = pauseText.getLocalBounds();
        pauseText.setOrigin({ (rect.left + rect.width) / 2.f, (rect.top + rect.height) / 2.f });
        pauseText.setPosition(rt.getView().getCenter());
        rt.draw(pauseText);
    }

    if (m_gameover)
    {
        sf::Text pauseText(std::string("You ") + (m_won ? "are a WinRar" : "failed") + " at " + (m_reason == Reason_Minesweeper ? "Minesweeper" : "Asteroids") + ".\nPress <Esc> to play again.", Application::GetSingleton().GetDefaultFont());
        pauseText.setCharacterSize(38u);
        pauseText.setOutlineColor(sf::Color::Black);
        pauseText.setOutlineThickness(2.f);
        auto rect = pauseText.getLocalBounds();
        pauseText.setOrigin({ (rect.left + rect.width) / 2.f, (rect.top + rect.height) / 2.f });
        pauseText.setPosition(rt.getView().getCenter());
        rt.draw(pauseText);

        pauseText.move(0, rect.top + rect.height + 20);
        pauseText.setCharacterSize(20);
        pauseText.setString("Your final scores are:\nAsteroids: " + std::to_string(m_asteroids.GetScore()) + "\nMinesweeper: " + std::to_string(int(m_minesweeper.GetTime())) + "s (on a " + std::to_string(m_minesweeper.GetCurSize()) + "x" + std::to_string(m_minesweeper.GetCurSize()) + " board)");
        rect = pauseText.getLocalBounds();
        pauseText.setOrigin({ (rect.left + rect.width) / 2.f, (rect.top + rect.height) / 2.f });
        rt.draw(pauseText);

        pauseText.setString("Change difficulty;\nMinesweeper board size: " + std::to_string(m_minesweeper.GetSize()) + " (up/down)");
        pauseText.move(0, rect.top + rect.height + 20);
        rect = pauseText.getLocalBounds();
        pauseText.setOrigin({ (rect.left + rect.width) / 2.f, (rect.top + rect.height) / 2.f });
        rt.draw(pauseText);
    }
}
