#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/ConvexShape.hpp>

namespace Asteroids
{

class Asteroid : public sf::Drawable, public sf::Transformable
{
public:
    Asteroid(float size);
    ~Asteroid();

    void Update();

    float GetSize() const;
    const sf::Vector2f& GetVelocity() const;

    void AddImpulse(const sf::Vector2f& impulse);
    void SetRotationSpeed(float rotSpeed);

    void draw(sf::RenderTarget&, sf::RenderStates) const;

private:
    float m_size, m_rotSpeed;
    sf::Vector2f m_velocity;
    sf::ConvexShape m_vertices;
};

}
