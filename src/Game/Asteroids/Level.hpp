#pragma once

namespace sf { class View; }

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "Asteroid.hpp"
#include "Player.hpp"

namespace Asteroids
{

class Level : public sf::Drawable
{
public:
    enum
    {
        k_maxAsteroidCount = 10,
        k_maxBulletCount = 15
    };

    Level();
    ~Level();

    void Update();
    void Reset();

    void FireBullet();
    const sf::Vector2f& GetPlayerPosition() const;

    /// \note The name is for sf::Drawable
    void draw(sf::RenderTarget& rt, sf::RenderStates states) const;

private:
    struct Bullet
    {
        sf::Vector2f Pos,
                     Vel;
        float Life;
    };

    std::vector<Asteroid> m_asteroids;
    std::vector<Bullet> m_bullets;
    Player m_player;

    float m_asteroidTimer;
    sf::VertexArray m_starfield1, m_starfield2;
};

}
