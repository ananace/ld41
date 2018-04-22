#include "IntroState.hpp"
#include "MenuState.hpp"

#include <Application.hpp>
#include <StateManager.hpp>

#include <SFML/Graphics/RectangleShape.hpp>

IntroState::IntroState()
    : m_time(0)
{
}
IntroState::~IntroState()
{

}

void IntroState::Update()
{
    m_time += GetStateManager().GetFrameTimeDT();

    rotate(m_time);

    if (m_time >= 10)
    {
        GetStateManager().PopState();
        GetStateManager().PushState<MenuState>();
    }
}

void IntroState::Draw(sf::RenderTarget& rt) const
{
    sf::RenderStates states;
    states.transform *= getTransform();

    sf::RectangleShape rect({500, 500});
    rect.setOrigin({250,250});
    rect.setFillColor(sf::Color::White);

    rt.draw(rect, states);
}

void IntroState::DrawUI(sf::RenderTarget&) const {}
