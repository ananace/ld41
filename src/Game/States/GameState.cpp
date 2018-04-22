#include "GameState.hpp"

#include <Application.hpp>
#include <Inputs.hpp>
#include <StateManager.hpp>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>

GameState::GameState()
    : m_paused(false)
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

    if (!m_paused)
    {
        m_asteroids.Update();
        m_minesweeper.Update();
    }
    else
    {
        float dt = GetStateManager().GetFrameTimeDT();
        m_pauseTime += dt;
        m_cumulativePauseTime += dt;
    }

    if (inp[Input_Cancel].IsPressStart())
        m_paused = !m_paused;
}

void GameState::Draw(sf::RenderTarget& rt) const
{
    auto preView = rt.getView();

    sf::View asView({ 0, 0, float(rt.getSize().x) / 2.f, float(rt.getSize().y) });
    asView.setViewport({ 0, 0, 0.5, 1 });
    sf::View msView({ 0, 0, float(rt.getSize().x) / 2.f, float(rt.getSize().y) });
    msView.setViewport({ 0.5, 0, 1, 1 });

    rt.setView(asView);
    m_asteroids.Draw(rt);

    rt.setView(msView);
    m_minesweeper.Draw(rt);

    rt.setView(preView);
}

void GameState::DrawUI(sf::RenderTarget& rt) const
{
    sf::View asView({ 0, 0, float(rt.getSize().x) / 2.f, float(rt.getSize().y) });
    asView.setViewport({ 0, 0, 0.5, 1 });
    sf::View msView({ 0, 0, float(rt.getSize().x) / 2.f, float(rt.getSize().y) });
    msView.setViewport({ 0.5, 0, 1, 1 });

    m_asteroids.DrawUI(rt);
    m_minesweeper.DrawUI(rt);

    if (m_paused)
    {
        sf::RectangleShape darken(rt.getView().getSize());
        darken.setFillColor({ 0, 0, 0, uint8_t(std::min(m_pauseTime, 1.f) * 128) });

        rt.draw(darken);

        sf::Text pauseText("[ Paused ]", Application::GetSingleton().GetDefaultFont());
        pauseText.setCharacterSize(38u);
        pauseText.setOutlineColor(sf::Color::Black);
        pauseText.setOutlineThickness(2.f);
        auto rect = pauseText.getLocalBounds();
        pauseText.setOrigin({ (rect.left + rect.width) / 2.f, (rect.top + rect.height) / 2.f });
        pauseText.setPosition(rt.getView().getCenter());
        rt.draw(pauseText);
    }
}
