#include "GameState.hpp"
#include "StateManager.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include "Minesweeper/Level.hpp"
#include "Asteroids/Level.hpp"

#include <random>

Asteroids::Level player;
Minesweeper::Level level({30, 30});
float rtime = 0;

GameState::GameState()
{
    level.setPosition({ 15, 15 });
    level.setScale(0.5f, 0.5f);
}
GameState::~GameState()
{

}

void GameState::Update()
{
    rtime += GetStateManager().GetFrameTimeDT();

    if (rtime >= 1.f)
    {
        rtime = 0;

        std::random_device dev;
        std::uniform_int_distribution<unsigned int> dist(0, 29);
        level.Reveal({ dist(dev), dist(dev) });
    }
    
    player.Update();
}

void GameState::Draw(sf::RenderTarget& rt) const
{
    auto preView = rt.getView();
    auto gameView = preView;

    auto viewSize = gameView.getSize();

    gameView.setViewport({ 0, 0, 1, 0.5 });
    gameView.setCenter({ viewSize.x / 2.f, viewSize.y / 4.f });
    rt.setView(gameView);

    gameView.setViewport({ 0, 0.5, 1, 0.5 });
    gameView.setCenter({ viewSize.x / 2.f, viewSize.y / 4.f + viewSize.y / 2.f });
    rt.setView(gameView);

    rt.setView(preView);
}

void GameState::DrawUI(sf::RenderTarget& rt) const
{
    rt.draw(level);
    auto view = rt.getView();
    view.setCenter(player.GetPlayerPosition());
    rt.setView(view);
    rt.draw(player);
    
}
