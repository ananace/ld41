#pragma once

#include <IState.hpp>

#include <SFML/Graphics/Texture.hpp>

class MenuState : public IState
{
public:
    MenuState();
    ~MenuState();

    void Update();

    void Draw(sf::RenderTarget&) const;
    void DrawUI(sf::RenderTarget&) const;

    const char* GetName() const { return "MenuState"; }

private:
    sf::Texture m_background;
};
