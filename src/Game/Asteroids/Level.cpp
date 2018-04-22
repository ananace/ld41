#include "Level.hpp"

#include <Application.hpp>
#include <Inputs.hpp>
#include <Math.hpp>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/View.hpp>

#include <random>

using namespace Asteroids;

Level::Level()
    : m_asteroidTimer(0)
    , m_starfield1(sf::Points, 0)
    , m_starfield2(sf::Points, 0)
{
    Reset();
}
Level::~Level()
{
    
}

void Level::Update()
{
    float dt = Application::GetSingleton().GetStateManager().GetFrameTimeDT();
    auto& inp = Application::GetSingleton().GetInputManager();
    m_asteroidTimer += dt;

    if (m_asteroidTimer >= 5.f)
    {
        m_asteroidTimer = 0;
        if (m_asteroids.size() < k_maxAsteroidCount)
        {
            std::random_device dev;
            Asteroid toAdd(std::uniform_real_distribution<float>(0.75f, 1.25f)(dev) * 37.5f);
            float ang = std::uniform_real_distribution<float>(0, Math::PI<float>() * 2.f)(dev);
            float force = std::uniform_real_distribution<float>(5, 20)(dev);
            toAdd.AddImpulse({ float(cos(ang) * force), float(sin(ang) * force) });

            // TODO: Create in a large circle around the player, in a player-heading direction

            m_asteroids.push_back(toAdd);
        }
    }

    if (inp[Input_Action].IsPressStart() && m_bullets.size() < k_maxBulletCount)
    {
        float ang = m_player.getRotation() * (Math::PI<float>() / 180.f);
        sf::Vector2f dir(cos(ang), sin(ang));
        m_bullets.push_back({ m_player.getPosition() + dir * 20.f, m_player.GetVelocity() + dir * 250.f, 0.f });
    }

    for (auto ait = m_asteroids.begin(); ait != m_asteroids.end();)
    {
        auto& obj = *ait;
        obj.Update();

        if (!obj.IsAlive())
            ait = m_asteroids.erase(ait);
        else
            ++ait;
    }
    for (auto bit = m_bullets.begin(); bit != m_bullets.end();)
    {
        auto& obj = *bit;
        obj.Pos += obj.Vel * dt;
        obj.Life += dt;

        if (obj.Life >= 5.f)
            bit = m_bullets.erase(bit);
        else
            ++bit;
    }
    m_player.Update();
    if (!sf::FloatRect(-1000, -1000, 2000, 2000).contains(m_player.getPosition()))
    {
        auto pos = m_player.getPosition();
        if (pos.x <= -1000 || pos.x >= 1000)
            pos.x = -pos.x;
        if (pos.y <= -1000 || pos.y >= 1000)
            pos.y = -pos.y;
        m_player.setPosition(pos);
    }

    // Collision detection
    for (auto ait = m_asteroids.cbegin(); ait != m_asteroids.cend();)
    {
        auto& obj = *ait;
        bool hit = false;

        for (auto bit = m_bullets.cbegin(); bit != m_bullets.cend();)
        {
            auto& bul = *bit;
            if (Math::distance(bul.Pos, obj.getPosition()) <= obj.GetSize())
            {
                m_bullets.erase(bit);
                hit = true;

                break;
            }

            ++bit;
        }

        if (!hit)
        {
            if (Math::distance(obj.getPosition(), m_player.getPosition()) <= obj.GetSize())
            {
                ++ait; // TODO: Explode player
            }
            else
                ++ait;
        }
        else
        {
            // TODO: Explode asteroid

            ait = m_asteroids.erase(ait);
        }
    }
}

void Level::Reset()
{
    m_asteroids.clear();
    m_bullets.clear();
    m_asteroids.reserve(k_maxAsteroidCount * 3);
    m_bullets.reserve(k_maxBulletCount);
    m_asteroidTimer = 0;

    std::random_device dev;
    std::uniform_real_distribution<float> size(-1000, 1000);

    m_starfield1.clear();
    m_starfield1.resize(1000);
    for (size_t i = 0; i < m_starfield1.getVertexCount(); ++i)
        m_starfield1[i] = sf::Vertex({ size(dev), size(dev) });

    m_starfield2.clear();
    m_starfield2.resize(500);
    for (size_t i = 0; i < m_starfield2.getVertexCount(); ++i)
        m_starfield2[i] = sf::Vertex({ size(dev), size(dev) }, { 128, 128, 128 });
}

const sf::Vector2f& Level::GetPlayerPosition() const
{
    return m_player.getPosition();
}

void Level::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    sf::RenderStates parallaxstate;
    rt.draw(m_starfield1, parallaxstate);
    parallaxstate.transform.translate(m_player.getPosition() / 4.f);
    rt.draw(m_starfield2, parallaxstate);

    for (auto& obj : m_asteroids)
        rt.draw(obj, states);

    sf::CircleShape bullet(2.5f, 6);
    bullet.setOrigin(2.5f, 2.5f);
    bullet.setFillColor(sf::Color::Transparent);
    bullet.setOutlineColor({ 255, 255, 0 });
    bullet.setOutlineThickness(1.f);
    for (auto& obj : m_bullets)
    {
        bullet.setPosition(obj.Pos);
        rt.draw(bullet, states);
    }

    rt.draw(m_player, states);
}
