#include "GameState.hpp"
#include "StateManager.hpp"

#include <SFML/Graphics/RenderTarget.hpp>


#include <random>

GameState::GameState()
{
}
GameState::~GameState()
{

}

void GameState::Update()
{
    m_asteroids.Update();
    m_minesweeper.Update();
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
}
