#pragma once

#include <IState.hpp>

#include <Minesweeper/Game.hpp>
#include <Asteroids/Game.hpp>

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
    enum Reason : uint8_t
    {
        Reason_Asteroids,
        Reason_Minesweeper,
    };

    Asteroids::Game m_asteroids;
    Minesweeper::Game m_minesweeper;

    bool m_paused, m_gameover, m_won;
    Reason m_reason;

    float m_pauseTime, m_cumulativePauseTime;
};
