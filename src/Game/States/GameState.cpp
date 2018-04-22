#include "GameState.hpp"
#include "StateManager.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include "Minesweeper/Game.hpp"
#include "Asteroids/Game.hpp"

#include <random>

Asteroids::Game asteroids;
Minesweeper::Game game;

GameState::GameState()
{
}
GameState::~GameState()
{

}

void GameState::Update()
{
    game.Update();
    asteroids.Update();
}

void GameState::Draw(sf::RenderTarget& rt) const
{
    auto preView = rt.getView();

    sf::View asView({ 0, 0, float(rt.getSize().x) / 2.f, float(rt.getSize().y) });
    sf::View msView({ 0, 0, float(rt.getSize().x) / 2.f, float(rt.getSize().y) });

    asView.setViewport({ 0, 0, 0.5, 1 });
    rt.setView(asView);
    asteroids.Draw(rt);

    msView.setViewport({ 0.5, 0, 1, 1 });
    rt.setView(msView);
    game.Draw(rt);

    rt.setView(preView);
}

void GameState::DrawUI(sf::RenderTarget& rt) const
{
    game.DrawUI(rt);
}
