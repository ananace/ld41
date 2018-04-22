#include "GameState.hpp"
#include "StateManager.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include "Minesweeper/Game.hpp"
#include "Asteroids/Level.hpp"

#include <random>

Asteroids::Level player;
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
    player.Update();
}

void GameState::Draw(sf::RenderTarget& rt) const
{
    auto preView = rt.getView();

    sf::View asView({ 0, 0, float(rt.getSize().x) / 2.f, float(rt.getSize().y) });
    sf::View msView({ 0, 0, float(rt.getSize().x) / 2.f, float(rt.getSize().y) });

    asView.setViewport({ 0, 0, 0.5, 1 });
    asView.setCenter(player.GetPlayerPosition());
    rt.setView(asView);
    rt.draw(player);

    msView.setViewport({ 0.5, 0, 1, 1 });
    rt.setView(msView);
    game.Draw(rt);

    rt.setView(preView);
}

void GameState::DrawUI(sf::RenderTarget& rt) const
{
    game.DrawUI(rt);
}
