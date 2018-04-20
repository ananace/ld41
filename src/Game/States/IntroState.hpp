#pragma once

#include <IState.hpp>

#include <SFML/Graphics/Transformable.hpp>

class IntroState : public IState, sf::Transformable
{
public:
    IntroState();
    ~IntroState();

    void Update();

    void Draw(sf::RenderTarget&) const;
    void DrawUI(sf::RenderTarget&) const;

    const char* GetName() const { return "IntroState"; }

private:
    float m_time;
};
