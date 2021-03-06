#include "Game.hpp"

#include <Application.hpp>
#include <Inputs.hpp>
#include <Math.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>

using namespace Asteroids;

Game::Game()
{
    
}
Game::~Game()
{
    
}

void Game::Update()
{
    auto& inp = Application::GetSingleton().GetInputManager();

    if (inp[Input_Action].IsPressStart())
        m_level.FireBullet();
    m_level.Update();
}

void Game::Reset()
{
    m_level.Reset();
}

bool Game::IsOver() const
{
    return m_level.IsDead();
}

int Game::GetScore() const
{
    return m_level.GetScore();
}

void Game::Draw(sf::RenderTarget& rt) const
{
    auto view = rt.getView();
    auto hvsize = view.getSize() / 4.f;
    auto lsize = sf::Vector2f(2000, 2000);
    sf::FloatRect viewLimit(-1000 + hvsize.x + 16, -1000 + hvsize.y * 2 + 16, lsize.x - hvsize.x * 4 - 32, lsize.y - hvsize.y * 4 - 32);

    view.setCenter(Math::constrain(viewLimit, m_level.GetPlayerPosition() - sf::Vector2f(hvsize.x, 0)) + sf::Vector2f(view.getSize().x / 4.f, 0));
    rt.setView(view);

    rt.draw(m_level);
}

void Game::DrawUI(sf::RenderTarget& rt) const
{
    sf::Text scoreText("Score: " + std::to_string(m_level.GetScore()), Application::GetSingleton().GetDefaultFont());
    scoreText.setCharacterSize(18u);
    scoreText.setOutlineColor(sf::Color::Black);
    scoreText.setOutlineThickness(2.f);
    auto rect = scoreText.getLocalBounds();
    scoreText.setOrigin({ (rect.left + rect.width) / 2.f, (rect.top + rect.height) / 2.f });
    scoreText.setPosition({ rt.getView().getCenter().x, rect.top + rect.height + 5 });
    rt.draw(scoreText);
}
