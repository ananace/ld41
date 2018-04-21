#pragma once

#include <IState.hpp>

class GameState : public IState
{
public:
    GameState();
    ~GameState();

    void Update();

    void Draw(sf::RenderTarget&) const;
    void DrawUI(sf::RenderTarget&) const;

    const char* GetName() const { return "GameState"; }

private:
    // Asteroids::Game m_asteroids;
    // Minesweeper::Game m_minesweeper;
};
