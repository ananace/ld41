#pragma once

#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

namespace Asteroids
{

class Player : public sf::Drawable, public sf::Transformable
{
public:
    Player();
    ~Player() = default;

    void Update();
    void AddImpulse(float force);

    void draw(sf::RenderTarget&, sf::RenderStates) const;

private:
    sf::ConvexShape m_shipShape, m_exhaustShape;
    sf::Vector2f m_velocity;

    float m_anim, m_burnTime;
};

}
