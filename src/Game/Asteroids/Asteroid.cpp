#include "Asteroid.hpp"

#include <Application.hpp>
#include <Math.hpp>

#include <random>

#include <cmath>

using namespace Asteroids;

Asteroid::Asteroid(float size)
    : m_size(size)
{
    std::random_device dev;
    setRotation(std::uniform_real_distribution<>(0.f, Math::PI<float>() * 2)(dev));
    m_rotSpeed = std::uniform_real_distribution<>(Math::PI<float>() * -0.5f, Math::PI<float>() * 1.5f)(dev);

    float pointCount = size * 1.1;
    m_vertices.setPointCount(int(pointCount * std::uniform_real_distribution<>(0.75f, 1.25f)(dev)));
    for (size_t i = 0; i < m_vertices.getPointCount(); ++i)
    {
        float ang = ((Math::PI<float>() * 2.f) / m_vertices.getPointCount()) * i;
        sf::Vector2f pos(cos(ang), sin(ang));

        m_vertices.setPoint(i, pos * m_size * std::uniform_real_distribution<float>(0.75f, 1.25f)(dev));
    }
    m_vertices.setOutlineColor(sf::Color::White);
    m_vertices.setOutlineThickness(1.5f);

    m_vertices.setOrigin(m_size, m_size);
}
Asteroid::~Asteroid() {}

void Asteroid::Update()
{
    float dt = Application::GetSingleton().GetStateManager().GetFrameTimeDT();

    rotate(m_rotSpeed * dt);
    move(m_velocity * dt);
}

float Asteroid::GetSize() const
{
    return m_size;
}
const sf::Vector2f& Asteroid::GetVelocity() const
{
    return m_velocity;
}

void Asteroid::AddImpulse(const sf::Vector2f& impulse)
{
    m_velocity += impulse;
}

void Asteroid::SetRotationSpeed(float rotSpeed)
{
    m_rotSpeed = rotSpeed;
}

void Asteroid::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    states.transform *= getTransform();

    rt.draw(m_vertices, states);
}
