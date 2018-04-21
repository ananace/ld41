#include "GameState.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

GameState::GameState()
{
}
GameState::~GameState()
{

}

void GameState::Update()
{

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

}
