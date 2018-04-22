#include "Player.hpp"

#include <Application.hpp>
#include <Inputs.hpp>
#include <Math.hpp>

#include <algorithm>

#include <cmath>

using namespace Asteroids;

Player::Player()
    : m_anim(0)
    , m_burnTime(0)
{
    m_shipShape.setPointCount(4);
    m_shipShape.setPoint(0, { 0, -20 });
    m_shipShape.setPoint(1, { 10, 10 });
    m_shipShape.setPoint(2, { 0, 5 });
    m_shipShape.setPoint(3, { -10, 10 });

    m_shipShape.setFillColor(sf::Color::Black);
    m_shipShape.setOutlineColor(sf::Color::White);
    m_shipShape.setOutlineThickness(1.5f);

    m_exhaustShape.setPointCount(8);
    m_exhaustShape.setPoint(0, { 10, 0 });
    m_exhaustShape.setPoint(1, { 10, 10 });
    m_exhaustShape.setPoint(2, { 3, 2 });
    m_exhaustShape.setPoint(3, { 0, 12 });
    m_exhaustShape.setPoint(4, { -3, 1 });
    m_exhaustShape.setPoint(5, { -10, 10 });
    m_exhaustShape.setPoint(6, { -10, 0 });
    m_exhaustShape.setPoint(7, { 0, -5 });

    m_exhaustShape.move(-17, 0);

    m_exhaustShape.setFillColor(sf::Color::Black);
    m_exhaustShape.setOutlineColor({ 16, 196, 255 });
    m_exhaustShape.setOutlineThickness(1.5f);

    m_shipShape.rotate(90);
    m_exhaustShape.rotate(90);
}

void Player::Update()
{
    float dt = Application::GetSingleton().GetStateManager().GetFrameTimeDT();
    auto& inp = Application::GetSingleton().GetInputManager();

    m_anim += dt;

    if (inp[Input_Up].IsPressed())
    {
        m_burnTime = std::max(0.5f, std::min(1.f, m_burnTime + dt * 0.75f));
        AddImpulse(m_burnTime * 50 * dt);
    }
    else
        m_burnTime = 0;

    float rot = (inp[Input_Right].CurrentValue - inp[Input_Left].CurrentValue) * 180;
    rotate(rot * dt);
    move(m_velocity * dt);
}
void Player::AddImpulse(float force)
{
    float ang = getRotation() * (Math::PI<float>() / 180.f);

    m_velocity += sf::Vector2f(cos(ang) * force, sin(ang) * force);
}

const sf::Vector2f& Player::GetVelocity() const
{
    return m_velocity;
}

void Player::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    auto& inp = Application::GetSingleton().GetInputManager();
    states.transform *= getTransform();

    rt.draw(m_shipShape, states);

    if (inp[Input_Up].IsPressed())
    {
        states.transform.scale(m_burnTime * (1+cos(m_anim*8)/10), m_burnTime * (1+cos(m_anim*8)/10));
        rt.draw(m_exhaustShape, states);
    }
}
